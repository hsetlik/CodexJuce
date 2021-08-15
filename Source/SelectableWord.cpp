/*
  ==============================================================================

    SelectableWord.cpp
    Created: 13 Aug 2021 6:52:48am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "SelectableWord.h"

Word::Word(std::string content, PhrasePair* pair) :
parentPair(pair),
text(content),
mouseIsOver(false),
linkedWord(nullptr)
{
    
}

void Word::resized()
{
    
}

void Word::paint(juce::Graphics &g)
{
    if(mouseIsOver)
    {
        g.fillAll(juce::Colours::lightblue);
    }
    else if (linkedWord != nullptr)
    {
        g.fillAll(juce::Colours::lightgrey);
    }
    g.setColour(juce::Colours::white);
    auto height = (float)getHeight();
    g.setFont(juce::Font(height));
    g.drawFittedText(juce::String(text), getLocalBounds(), juce::Justification::centred, 1);
}

int Word::lengthForFontHeight(int height)
{
    auto font = juce::Font((float)height);
    return font.getStringWidth(juce::String(text));
}

void Word::mouseEnter(const juce::MouseEvent &e)
{
    mouseIsOver = true;
    repaint();
}
void Word::mouseExit(const juce::MouseEvent &e)
{
    mouseIsOver = false;
    repaint();
}

void NativeWord::itemDropped(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails)
{
    if(auto target = dynamic_cast<Word*>(dragSourceDetails.sourceComponent.get()))
    {
        target->linkToWord(this);
        parentPair->addNtaPair(text, target->text);
        getParentComponent()->getParentComponent()->repaint();
    }
}
//==============================================================================

Phrase::Phrase(std::string fullPhrase, PhrasePair* pair) : parentPair(pair)
{
    auto exp = std::regex("\\w+");
    auto vWords = stdu::matchesAsVector(fullPhrase, exp);
    for(auto w : vWords)
    {
        addAndMakeVisible(words.add(new Word(w, parentPair)));
    }
}

void Phrase::resized()
{
    auto height = getHeight();
    int currentX = 0;
    for (auto word : words)
    {
        auto wLength = word->lengthForFontHeight(height);
        word->setBounds(currentX, 0, wLength, height);
        currentX += wLength + SPACE_WIDTH;
    }
}

int Phrase::getPhraseWidth(int height)
{
    int currentX = 0;
    for (auto word : words)
    {
        currentX += word->lengthForFontHeight(height) + SPACE_WIDTH;
    }
    return currentX;
}
//==============================================================================
PhraseScreen::PhraseScreen(TextEntryScreen* screen) :
linkedScreen(screen),
nativePhrase(nullptr),
targetPhrase(nullptr),
phrasePair(nullptr)
{
    updatePhrases();
    backButton.setButtonText("Back");
    addAndMakeVisible(&backButton);
}

void PhraseScreen::updatePhrases()
{
    auto phrases = linkedScreen->getEntry();
    phrasePair.reset(new PhrasePair(phrases.first, phrases.second));
    nativePhrase.reset(new NativePhrase(phrases.first, &*phrasePair));
    targetPhrase.reset(new TargetPhrase(phrases.second, &*phrasePair));
    
    
    addAndMakeVisible(&*nativePhrase);
    addAndMakeVisible(&*targetPhrase);
    
    resized();
}

void PhraseScreen::visibilityChanged()
{
    if(isVisible())
        updatePhrases();
}

void PhraseScreen::resized()
{
    auto dX = getHeight() / 16;
    auto nLength = nativePhrase->getPhraseWidth(dX);
    auto tLength = targetPhrase->getPhraseWidth(dX);
    
    targetPhrase->setBounds(2 * dX, 2 * dX, tLength, dX);
    nativePhrase->setBounds(2 * dX, 6 * dX, nLength, dX);
    backButton.setBounds(2 * dX, 10 * dX, 3 * dX, 2 * dX);
}
void PhraseScreen::paint(juce::Graphics& g)
{
    getNtaPaths(ntaPaths);
    auto wide = juce::PathStrokeType(6.0f);
    auto narrow = juce::PathStrokeType(2.8f);
    int i = 0;
    for(auto link : ntaPaths)
    {
        g.setColour(juce::Colours::black);
        g.fillEllipse(link.nCircle.toFloat());
        g.fillEllipse(link.tCircle.toFloat());
        g.strokePath(link.path, wide);
        g.setColour(ColorUtil::shadesDarker(juce::Colours::green, i, 0.3f));
        g.strokePath(link.path, narrow);
        g.fillEllipse(link.nCircle.toFloat().reduced(1.5f));
        g.fillEllipse(link.tCircle.toFloat().reduced(1.5f));
        ++i;
    }
}
NtaLinkBounds PhraseScreen::ntaPathFor(std::string native, std::string target)
{
    NtaLinkBounds nlb;
    juce::Path path;
    auto nBounds = nativePhrase->wordBounds(native);
    auto tBounds = targetPhrase->wordBounds(target);
    auto nX = (float)nBounds.getX() + (nBounds.getWidth() / 2);
    auto nY = (float)nBounds.getY() - (nBounds.getHeight() * 0.25f);
    auto tX = (float)tBounds.getX() + (tBounds.getWidth() / 2);
    auto tY = (float)tBounds.getY() + (tBounds.getHeight() * 1.25f);
    auto largerX = (nX > tX)? nX : tX;
    auto smallerX = (nX > tX)? tX : nX;
    auto smallerY = (largerX == tX) ? nY : tY;
    auto largerY = (largerX == tX) ? tY : nY;
    float centerX = smallerX + ((largerX - smallerX) / 2);
    float centerY = tY + ((nY - tY) / 2);
    nlb.nCircle = squareAround(largerX, largerY);
    nlb.tCircle = squareAround(smallerX, smallerY);
    path.startNewSubPath(smallerX, smallerY);
    path.quadraticTo(smallerX, centerY, centerX, centerY);
    path.quadraticTo(largerX, centerY, largerX, largerY);
    nlb.path = path;
    return nlb;
}

void PhraseScreen::getNtaPaths(std::vector<NtaLinkBounds>& paths)
{
    paths.clear();
    for(auto pair : phrasePair->ntaPairs)
    {
        paths.push_back(ntaPathFor(pair.first, pair.second));
    }
}
