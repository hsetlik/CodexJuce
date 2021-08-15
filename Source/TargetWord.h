/*
  ==============================================================================

    SelectableWord.h
    Created: 13 Aug 2021 6:52:48am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include "TextEntryComponent.h"

//Represents a word from the target language that can be dragged and dropped onto a native word

class TargetWord: public juce::Component
{
public:
    TargetWord(std::string content);
    const std::string text;
    void resized() override;
    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;
};
