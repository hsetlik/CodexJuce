/*
  ==============================================================================

    TextEntryComponent.cpp
    Created: 13 Aug 2021 6:53:06am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "TextEntryComponent.h"


TextEntryScreen::TextEntryScreen()
{
    nextButton.setButtonText("Next");
    addAndMakeVisible(&nextButton);
    addAndMakeVisible(&nativeEditor);
    addAndMakeVisible(&targetEditor);
    nativeEditor.setText("Example native phrase");
    targetEditor.setText("Example target phrase");
}

void TextEntryScreen::resized()
{
    cUtil::setByRelativeY(nativeEditor, *this, 16, 2, 3, 12, 1);
    cUtil::setByRelativeY(targetEditor, *this, 16, 2, 5, 12, 1);
    cUtil::setByRelativeY(nextButton, *this, 16, 2, 8, 4, 2);
}

void TextEntryScreen::paint(juce::Graphics &g)
{
    
}


