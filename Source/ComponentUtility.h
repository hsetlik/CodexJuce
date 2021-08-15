/*
  ==============================================================================

    ComponentUtility.h
    Created: 13 Aug 2021 7:04:16am
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace cUtil
{
    void setByRelativeX(juce::Component& child, juce::Component& parent, int widthFraction, int x, int y, int w, int h);
    void setByRelativeY(juce::Component& child, juce::Component& parent, int heightFraction, int x, int y, int w, int h);
}
