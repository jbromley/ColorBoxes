//
//  Bomb.h
//  ColorBoxes
//
//  Created by Jay Bromley on 1/27/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__Bomb__
#define __ColorBoxes__Bomb__

#include <iostream>
#include "PhysicsEntity.h"


class ColorBoxesEngine;


class Bomb : public PhysicsEntity
{
public:
    Bomb(float x, float y, float blastRadius, float strength,
         float fuseLength, ColorBoxesEngine* engine);
    virtual ~Bomb();
    
    virtual void update(long millisElapsed);
    virtual void render();
    virtual bool done() const;
    
protected:
    virtual float boundingRadius() const;
    virtual b2Shape* makeShape();
    
private:
    void explode();
    
    float radius_;
    float blastRadius_;
    float maxImpulse_;
    float countdown_;
};


#endif /* defined(__ColorBoxes__Bomb__) */
