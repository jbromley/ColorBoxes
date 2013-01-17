//
//  OpenGLDraw.h
//  ColorBoxes
//
//  Created by Jay Bromley on 1/17/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__OpenGLDraw__
#define __ColorBoxes__OpenGLDraw__

#include <iostream>
#include <string>
#include <Box2D/Box2D.h>
#include "GLColor.h"


namespace ogl
{

    void drawSolidPolygon(const b2Vec2* vertices, int vertexCount,
                          const GLColor& borderColor, const GLColor& fillColor);
    void drawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis,
                         const GLColor& borderColor, const GLColor& fillColor);
    void drawSegment(const b2Vec2& p1, const b2Vec2& p2, float thickness,
                     const GLColor& color);
    void drawPoint(const b2Vec2& p, float32 size, const GLColor& color);
    void drawString(int x, int y, const std::string& text, const GLColor& color);
};


#endif /* defined(__ColorBoxes__OpenGLDraw__) */
