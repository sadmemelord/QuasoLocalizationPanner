
#include "CustomColors.h"

juce::Colour CustomColors::findCustomColourFromIndex(int index)
{
    switch (index)
    {
    case 1:
            return CustomColors::palette1;
    case 2:
            return CustomColors::palette2;
    case 3:
            return CustomColors::palette3;
    case 4:
            return CustomColors::palette4;
    case 5:
            return CustomColors::palette5;
    case 6:
            return CustomColors::palette6;
    case 7:
            return CustomColors::palette7;
    case 8:
            return CustomColors::palette8;
    case 9:
            return CustomColors::palette9;
    case 10:
            return CustomColors::palette10;
    case 11:
            return CustomColors::palette11;
    case 12:
            return CustomColors::palette12;
    case 13:
            return CustomColors::palette13;
    case 14:
            return CustomColors::palette14;
    case 15:
            return CustomColors::palette15;
    case 16:
            return CustomColors::palette16;
    default:
            return juce::Colours::black;
    }

}
