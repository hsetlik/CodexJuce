/*
  ==============================================================================

    DataStructures.cpp
    Created: 13 Aug 2021 4:59:58pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#include "DataStructures.h"

PhrasePair::PhrasePair(std::string n, std::string t) :
nativeFull(n),
targetFull(n)
{
    auto exp = std::regex("\\w+");
    nativeWords = stdu::matchesAsVector(nativeFull, exp);
    targetWords = stdu::matchesAsVector(targetFull, exp);
}

juce::ValueTree PhrasePair::getClozeTreeFor(std::string targetWord)
{
    juce::ValueTree tree
    {
        "ClozePair", {{"id", targetWord.c_str() + "_cloze"}},
        {
            {"NativeFull", {{"value", nativeFull.c_str()}}},
            {"TargetFull", {{"value", targetFull.c_str()}}},
            {"ClozeWord", {{"value", targetWord.c_str()}}}
        }
    };
    return tree;
}

juce::ValueTree PhrasePair::getTreeFor(std::pair<std::string, std::string> pair)
{
    juce::ValueTree tree
    {
        "NtaPair", {{"id", std::string(pair.first + "_word").c_str()}},
        {
            { "NativeWord", {{"value", pair.first.c_str()}}},
            { "TargetWord", {{"value", pair.second.c_str()}}}
        }
    };
    return tree;
}

juce::ValueTree PhrasePair::getNtaTree()
{
    auto phraseName = nativeFull + "_phrase";
    juce::ValueTree tree
    {
        "NtaPairs", {{"phrase", phraseName.c_str()}}
    };
    for(auto pair : ntaPairs)
    {
        auto child = getTreeFor(pair);
        tree.appendChild(child, nullptr);
    }
    return tree;
}

juce::ValueTree PhrasePair::getClozeTree()
{
    auto phraseName = nativeFull + "_phrase";
    juce::ValueTree tree
    {
        "ClozePairs", {{"phrase", phraseName.c_str()}}
    };
    for(auto word : clozeWords)
    {
        auto child = getClozeTreeFor(word);
        tree.appendChild(child, nullptr);
    }
    return tree;
}
