//
//  Polygon.h
//  ColorBoxes
//
//  Created by Jay Bromley on 1/15/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__Polygon__
#define __ColorBoxes__Polygon__

#include <iostream>
#include "PhysicsEntity.h"


class ColorBoxesEngine;


class Polygon : public PhysicsEntity
{
public:
    Polygon(float x, float y, int numVertices, ColorBoxesEngine* engine);
    virtual ~Polygon();
    
protected:
    virtual float boundingRadius() const;
    virtual b2Shape* makeShape();
    
private:
    float radius_;
};

#endif /* defined(__ColorBoxes__Polygon__) */
