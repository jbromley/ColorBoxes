//
//  Shape.h
//  ColorBoxes
//
//  Created by Jay Bromley on 1/14/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__Shape__
#define __ColorBoxes__Shape__

#include <iostream>
#include <Box2D/Box2D.h>
#include "GLColor.h"


class ColorBoxesEngine;

class Shape
{
public:
    Shape(int numVertices, ColorBoxesEngine* engine);
    virtual ~Shape();
    
    virtual void update(long timeElapsed) = 0;
    virtual void render();
    bool done() const;
    
protected:
    virtual float shapeHeight() const = 0;
    virtual b2Shape* makeShape(const b2Vec2& center) = 0;
    void makeBody(const b2Vec2& center);
    void killBody();
    
    b2Body* body_;
    int numVertices_;
    GLColor borderColor_;
    GLColor fillColor_;
    ColorBoxesEngine* engine_;
};

#endif /* defined(__ColorBoxes__Shape__) */
