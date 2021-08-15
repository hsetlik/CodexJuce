/*
  ==============================================================================

    StlUtil.h
    Created: 13 Aug 2021 12:29:34pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <regex>

struct stdu
{
    //! basic utility that splits a string into regex matches and returns them as a vector
    static std::vector<std::string> matchesAsVector(std::string body, std::regex reg)
    {
        std::vector<std::string> strings;
        auto results = std::smatch{};
        for (std::sregex_iterator it = std::sregex_iterator(body.begin(), body.end(), reg);
                 it != std::sregex_iterator(); it++)
        {
            std::smatch match;
            match = *it;
            auto str = match.str(0);
            strings.push_back(str);
        }
        return strings;
    }
};
