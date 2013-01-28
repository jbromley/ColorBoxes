//
//  ExplosionCallback.h
//  ColorBoxes
//
//  Created by Jay Bromley on 1/24/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__ExplosionCallback__
#define __ColorBoxes__ExplosionCallback__

#include <iostream>
#include <Box2D/Box2D.h>


class ExplosionCallback : public b2QueryCallback
{
public:
    ExplosionCallback(const b2Vec2& origin, float blastRadius, float impulse);
    virtual ~ExplosionCallback();

    virtual bool ReportFixture(b2Fixture* fixture);

private:
    b2Vec2 origin_;
    float blastRadius_;
    float impulse_;
};

#endif /* defined(__ColorBoxes__ExplosionCallback__) */
