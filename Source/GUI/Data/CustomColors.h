/*
  ==============================================================================

    CustomColors.h
    Created: 26 Apr 2023 4:36:37pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace CustomColors {

   const juce::Colour darkerGrey { 0xff101214};
   const juce::Colour darkGrey { 0xff262422 };
   const juce::Colour lightGrey{ 0xffa6a6a6 };

   const juce::Colour palette1{ 0xfff44336 };
   const juce::Colour palette2{ 0xffe81e63 };
   const juce::Colour palette3{ 0xff9c27b0 };
   const juce::Colour palette4{ 0xff673ab7 };

   const juce::Colour palette5{ 0xff3f51b5 };
   const juce::Colour palette6{ 0xff2196f3 };
   const juce::Colour palette7{ 0xff03a9f4 };
   const juce::Colour palette8{ 0xff00bcd4 };

   const juce::Colour palette9{ 0xff009688 };
   const juce::Colour palette10{ 0xff4caf50 };
   const juce::Colour palette11{ 0xff8bc34a };
   const juce::Colour palette12{ 0xffcddc39 };

   const juce::Colour palette13{ 0xffffeb3b };
   const juce::Colour palette14{ 0xffffc107 };
   const juce::Colour palette15{ 0xffff9800 };
   const juce::Colour palette16{ 0xffff5722 };

   juce::Colour findCustomColourFromIndex(int index);
}


