//
//  GLColor.h
//  ColorBoxes
//
//  Created by Jay Bromley on 12/28/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__GLColor__
#define __ColorBoxes__GLColor__

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <SDL/SDL.h>


struct GLColor
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
    
    GLColor();
    GLColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0);
    
    GLColor lighten(float factor) const;
    SDL_Color toSDLColor() const;
    
    static GLColor black();
    static GLColor white();
    static GLColor red();
    static GLColor green();
    static GLColor blue();
    static GLColor yellow();
    static GLColor magenta();
    static GLColor cyan();
    static GLColor randomColor();
    static GLColor randomRGBColor();
    
    bool operator==(const GLColor& other) const;
    bool operator!=(const GLColor& other) const;
    
private:
    GLfloat clamp(GLfloat& value) const;
};

#endif /* defined(__ColorBoxes__GLColor__) */
