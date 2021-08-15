/*
  ==============================================================================

    DataStructures.h
    Created: 13 Aug 2021 4:59:58pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include "StlUtil.h"
enum class CardType
{
    NTA,
    Cloze,
    TypeFromAudio,
    FullPhrase
};



//Class to contain all the info for a pair of phrases and all associated cards
class PhrasePair
{
public:
    PhrasePair(std::string n, std::string t);
    ~PhrasePair()
    {
        for(auto it = ntaPairs.begin(); it != ntaPairs.end(); ++it)
        {
            std::cout << "Native: " << it->first << " Target: " << it->second << std::endl;
        }
    }
    const std::string nativeFull;
    const std::string targetFull;
    void addNtaPair(std::string n, std::string t)
    {
        removeNtaPair(n);
        ntaPairs[n] = t;
        auto tree = getNtaTree();
        auto str = tree.toXmlString();
        printf("%s\n", str.toRawUTF8());
    }
    void removeNtaPair(std::string native)
    {
        for(auto it = ntaPairs.begin(); it != ntaPairs.end(); ++it)
        {
            if(it->first == native)
            {
                ntaPairs.erase(it);
                return;
            }
        }
    }
    std::map<std::string, std::string> ntaPairs;
    std::vector<std::string> clozeWords;
    juce::ValueTree getTreeFor(std::pair<std::string, std::string> pair);
    juce::ValueTree getClozeTreeFor(std::string targetWord);
    juce::ValueTree getNtaTree();
    juce::ValueTree getClozeTree();
private:
    std::vector<std::string> nativeWords;
    std::vector<std::string> targetWords;
    
   //TODO: functions for saving this info to a file and on constructing this class from that file
};
