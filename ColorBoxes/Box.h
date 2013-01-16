//
//  Box.h
//  ColorBoxes
//
//  Created by Jay Bromley on 12/27/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__Box__
#define __ColorBoxes__Box__

#include <iostream>
#include <Box2D/Box2D.h>
#include "Shape.h"
#include "GLColor.h"


class ColorBoxesEngine;

class Box : public Shape
{
public:
    Box(float x, float y, ColorBoxesEngine* engine);
    virtual ~Box();
    
protected:
    virtual float shapeHeight() const;
    virtual b2Shape* makeShape();
    
private:    
    float w_;
    float h_;
};

#endif /* defined(__ColorBoxes__Box__) */
