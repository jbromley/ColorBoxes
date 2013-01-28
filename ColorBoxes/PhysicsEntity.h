//
//  PhysicsEntity.h
//  ColorBoxes
//
//  Created by Jay Bromley on 1/14/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__PhysicsEntity__
#define __ColorBoxes__PhysicsEntity__

#include <iostream>
#include <Box2D/Box2D.h>
#include "GLColor.h"


class ColorBoxesEngine;

class PhysicsEntity
{
public:
    PhysicsEntity(int numVertices, ColorBoxesEngine* engine);
    virtual ~PhysicsEntity();
    
    virtual void update(long millisElapsed);
    virtual void render();
    virtual bool done() const;
    
protected:
    virtual float boundingRadius() const = 0;
    virtual b2Shape* makeShape() = 0;
    void makeBody(const b2Vec2& center);
    void killBody();
    
    b2Body* body_;
    int numVertices_;
    GLColor borderColor_;
    GLColor fillColor_;
    ColorBoxesEngine* engine_;
};

#endif /* defined(__ColorBoxes__PhysicsEntity__) */
