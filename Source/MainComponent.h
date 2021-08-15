#pragma once

#include <JuceHeader.h>
#include "SelectableWord.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainScreen :
public juce::Component,
public juce::Button::Listener
{
public:
    MainScreen();
    void buttonClicked(juce::Button* b) override;
    void resized() override;
private:
    TextEntryScreen tScreen;
    PhraseScreen pScreen;
};



class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    MainScreen mScreen;
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
