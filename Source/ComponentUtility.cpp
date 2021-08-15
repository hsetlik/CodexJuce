/*
  ==============================================================================

    ComponentUtility.cpp
    Created: 13 Aug 2021 7:04:16am
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "ComponentUtility.h"

void cUtil::setByRelativeX(juce::Component &child, juce::Component &parent, int widthFraction, int x, int y, int w, int h)
{
    auto dX = parent.getWidth() / widthFraction;
    child.setBounds(x * dX, y * dX, w * dX, h * dX);
}

void cUtil::setByRelativeY(juce::Component& child, juce::Component& parent, int heightFraction, int x, int y, int w, int h)
{
    auto dY = parent.getWidth() / heightFraction;
    child.setBounds(x * dY, y * dY, w * dY, h * dY);
}
