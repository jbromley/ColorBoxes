//
//  GLColor.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 12/28/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#include "GLColor.h"
#include "Utilities.h"


GLColor::GLColor()
: r(0.0f), g(0.0f), b(0.0f), a(0.0f)
{
}

GLColor::GLColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha /*= 1.0*/)
: r(clamp(red)), g(clamp(green)), b(clamp(blue)), a(clamp(alpha))
{
    
}

GLColor
GLColor::lighten(float factor) const
{
    clamp(factor);
    return GLColor(r * factor, g * factor, b * factor, a * factor);
}

SDL_Color
GLColor::toSDLColor() const
{
    SDL_Color sdlColor = {static_cast<Uint8>(r * 255), static_cast<Uint8>(g * 255),
        static_cast<Uint8>(b * 255)};
    return sdlColor;
}

bool
GLColor::operator==(const GLColor& other) const
{
    return !(*this != other);
}

bool
GLColor::operator!=(const GLColor& other) const
{
    return (r != other.r || g != other.g || b != other.b || a != other.a);
}


GLColor
GLColor::black()
{
    static GLColor blackInstance(0.0f, 0.0f, 0.0f, 1.0f);
    return blackInstance;
}

GLColor
GLColor::white()
{
    static GLColor whiteInstance(1.0f, 1.0f, 1.0f, 1.0f);
    return whiteInstance;
}

GLColor
GLColor::randomColor()
{
    return GLColor(randomFloat(), randomFloat(), randomFloat(), randomFloat());
}

GLColor
GLColor::randomRGBColor()
{
    return GLColor(randomFloat(), randomFloat(), randomFloat(), 1.0);
}

GLfloat
GLColor::clamp(GLfloat& value) const
{
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }
    return value;
}
