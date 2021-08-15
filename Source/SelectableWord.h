/*
  ==============================================================================

    SelectableWord.h
    Created: 13 Aug 2021 6:52:48am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include "TextEntryComponent.h"
#include "DataStructures.h"
#define SPACE_WIDTH 11

class Word : public juce::Component
{
public:
    Word(std::string content, PhrasePair* parent);
    PhrasePair* const parentPair;
    const std::string text;
    void resized() override;
    void paint(juce::Graphics& g) override;
    int lengthForFontHeight(int height);
    void mouseEnter(const juce::MouseEvent& e) override;
    void mouseExit(const juce::MouseEvent& e) override;
    bool mouseIsOver;
    void linkToWord(Word* link)
    {
        if(link == linkedWord)
            return;
        linkedWord = link;
        linkedWord->linkToWord(this);
        repaint();
    }
    void severLink()
    {
        if(linkedWord == nullptr)
            return;
        auto prevLink = linkedWord;
        linkedWord = nullptr;
        prevLink->severLink();
        repaint();
    }
protected:
    Word* linkedWord;
};

struct NtaLinkBounds
{
    juce::Rectangle<int> nCircle;
    juce::Rectangle<int> tCircle;
    juce::Path path;
};

class TargetWord : public Word
{
public:
    TargetWord(std::string content, PhrasePair* pair) : Word(content, pair)
    {
        //setInterceptsMouseClicks(true, true);
    }
    void mouseDown(const juce::MouseEvent &e) override
    {
        if(e.mods.isRightButtonDown())
        {
            if(linkedWord != nullptr)
            {
                auto nString = linkedWord->text;
                severLink();
                parentPair->removeNtaPair(nString);
                getParentComponent()->getParentComponent()->repaint();
            }
        }
        if(linkedWord == nullptr)
            juce::DragAndDropContainer::findParentDragContainerFor(this)->startDragging(juce::String(text), this);
    }
};

class NativeWord : public Word, public juce::DragAndDropTarget
{
public:
    NativeWord(std::string content, PhrasePair* pair) : Word(content, pair)
    {
    }
    void itemDropped(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;
    void itemDragEnter(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override{}
    void itemDragExit(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override{}
    void itemDragMove(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override{}
    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override
    {
        return true;
    }
};
//===================================================================
class Phrase : public juce::Component
{
public:
    Phrase(std::string fullString, PhrasePair* parent);
    PhrasePair* const parentPair;
    int getPhraseWidth(int height);
    void resized() override;
    juce::Rectangle<int> wordBounds(std::string word)
    {
        for(auto w : words)
        {
            if(w->text == word)
            {
                auto wBounds = w->getBounds();
                auto sBounds = getBounds();
                return juce::Rectangle<int>(
                                            sBounds.getX() + wBounds.getX(),
                                            sBounds.getY() + wBounds.getY(),
                                            wBounds.getWidth(),
                                            wBounds.getHeight()
                                            );
            }
                
        }
        return {0, 0, 0, 0};
    }
    std::vector<std::string> getWords()
    {
        std::vector<std::string> vec;
        for(auto w : words)
            vec.push_back(w->text);
        return vec;
    }
protected:
    juce::OwnedArray<Word> words;
};

class TargetPhrase : public Phrase
{
public:
    TargetPhrase(std::string content, PhrasePair* parent) : Phrase(content, parent)
    {
        words.clear(true);
        auto exp = std::regex("\\w+");
        auto vWords = stdu::matchesAsVector(content, exp);
        for(auto w : vWords)
        {
            addAndMakeVisible(words.add(new TargetWord(w, parentPair)));
        }
    }
};

class NativePhrase : public Phrase
{
public:
    NativePhrase(std::string content, PhrasePair* parent) : Phrase(content, parent)
    {
        words.clear(true);
        auto exp = std::regex("\\w+");
        auto vWords = stdu::matchesAsVector(content, exp);
        for(auto w : vWords)
        {
            addAndMakeVisible(words.add(new NativeWord(w, parentPair)));
        }
    }
};



//=================================================
class PhraseScreen :
public juce::Component,
public juce::DragAndDropContainer
{
public:
    PhraseScreen(TextEntryScreen* screen);
    TextEntryScreen* const linkedScreen;
    void resized() override;
    void paint(juce::Graphics& g) override;
    void updatePhrases();
    void visibilityChanged() override;
    std::unique_ptr<NativePhrase> nativePhrase;
    std::unique_ptr<TargetPhrase> targetPhrase;
    std::unique_ptr<PhrasePair> phrasePair;
    juce::TextButton backButton;
    void getNtaPaths(std::vector<NtaLinkBounds>& paths);
    std::vector<NtaLinkBounds> ntaPaths;
    NtaLinkBounds ntaPathFor(std::string native, std::string target);
private:
    static juce::Rectangle<int> squareAround(int xCenter, int yCenter, int radius = 6)
    {
        auto x = xCenter - radius;
        auto y = yCenter - radius;
        auto width = 2 * radius;
        return juce::Rectangle<int>(x, y, width, width);
    }
};

