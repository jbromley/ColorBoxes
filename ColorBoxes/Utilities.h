//
//  Utilities.h
//  ColorBoxes
//
//  Created by Jay Bromley on 12/27/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#ifndef ColorBoxes_Utilities_h
#define ColorBoxes_Utilities_h

#include "SDL/SDL.h"

template <typename T>
T map(T value, T minFrom, T maxFrom, T minTo, T maxTo)
{
    return minTo + ((value - minFrom) / (maxFrom - minFrom)) * (maxTo - minTo);
}

int nextPowerOfTwo(int x);

Uint32
convertRGBAToColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

void
convertColorToRGBA(Uint32 color, Uint8* r, Uint8* g, Uint8* b, Uint8* a);

Uint32
lightenColor(Uint32 color, float factor);

Uint32
randomColor();

Uint32
randomRGBColor();

float
randomFloat();

float
randomFloat(float minValue, float maxValue);

float
roundToInt(float value);

#endif
