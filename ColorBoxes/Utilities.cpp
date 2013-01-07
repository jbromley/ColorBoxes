//
//  Utilities.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 12/27/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#include <cmath>
#include <cstdlib>
#include <limits>
#include "Utilities.h"


int nextPowerOfTwo(int x)
{
	double logbase2 = log(x) / log(2);
	return round(pow(2, ceil(logbase2)));
}

Uint32
convertRGBAToColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return a + (b << 8) + (g << 16) + (r << 24);
}

void
convertColorToRGBA(Uint32 color, Uint8* r, Uint8* g, Uint8* b, Uint8* a)
{
    *a = color & 0xff;
    color >>= 8;
    *b = color & 0xff;
    color >>= 8;
    *g = color & 0xff;
    color >>= 8;
    *r = color & 0xff;
}

Uint32
lightenColor(Uint32 color, float factor)
{
    Uint8 r, g, b, a;
    convertColorToRGBA(color, &r, &g, &b, &a);
    r *= factor;
    g *= factor;
    b *= factor;
    a *= factor;
    return convertRGBAToColor(r, g, b, a);
}

float
randomFloat()
{
#ifdef __APPLE__
    return static_cast<float>(arc4random()) / UINT32_MAX;
#else
    return static_cast<float>(random()) / std::numeric_limits<long int>::max();
#endif
}

float
randomFloat(float minValue, float maxValue)
{
    return minValue + (randomFloat() * (maxValue - minValue));
}

float
roundToInt(float value)
{
    return floor(value + 0.5f);
}

Uint32
randomColor()
{
#ifdef __APPLE__
    Uint8 r = arc4random_uniform(256);
    Uint8 g = arc4random_uniform(256);
    Uint8 b = arc4random_uniform(256);
    Uint8 a = arc4random_uniform(256);
#else
    Uint8 r = random() % 256;
    Uint8 g = random() % 256;
    Uint8 b = random() % 256;
    Uint8 a = random() % 256;
#endif
    return convertRGBAToColor(r, g, b, a);
}

Uint32
randomRGBColor()
{
#ifdef __APPLE__
    Uint8 r = arc4random_uniform(256);
    Uint8 g = arc4random_uniform(256);
    Uint8 b = arc4random_uniform(256);
#else
    Uint8 r = random() % 256;
    Uint8 g = random() % 256;
    Uint8 b = random() % 256;
#endif
    Uint8 a = 255;
    return convertRGBAToColor(r, g, b, a);
}
