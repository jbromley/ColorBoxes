//
//  Circle.h
//  ColorBoxes
//
//  Created by Jay Bromley on 1/15/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__Circle__
#define __ColorBoxes__Circle__

#include <iostream>
#include "Shape.h"


class ColorBoxesEngine;


class Circle : public Shape
{
public:
    Circle(float x, float y, ColorBoxesEngine* engine);
    virtual ~Circle();
    virtual void render();
    
protected:
    virtual float shapeHeight() const;
    virtual b2Shape* makeShape();
    
private:
    float radius_;
};

#endif /* defined(__ColorBoxes__Circle__) */
