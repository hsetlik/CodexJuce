/*
  ==============================================================================

    TextEntryComponent.h
    Created: 13 Aug 2021 6:53:06am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ComponentUtility.h"
#include "StlUtil.h"
#include "Color.h"
class TextEntryScreen : public juce::Component
{
public:
    TextEntryScreen();
    //juce overrides
    void resized() override;
    void paint(juce::Graphics &g) override;
    //return the entered values in pair format Native, Target
    std::pair<std::string, std::string> getEntry()
    {
        return std::make_pair(nativeEditor.getText().toStdString(), targetEditor.getText().toStdString());
    }
    juce::TextButton nextButton;
private:
    juce::TextEditor nativeEditor;
    juce::TextEditor targetEditor;
    
};
