//
//  ExplosionCallback.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 1/24/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#include "ExplosionCallback.h"

ExplosionCallback::ExplosionCallback(const b2Vec2& origin)
: origin_(origin)
{
}

ExplosionCallback::~ExplosionCallback()
{
}

bool
ExplosionCallback::ReportFixture(b2Fixture* fixture)
{
    const float MAX_DISTANCE = 20.0f;
    const float MAX_FORCE = 2000.0f;
    
    b2Body* body = fixture->GetBody();
    b2Vec2 bodyPos = body->GetPosition();
        
    float distance = b2Distance(bodyPos, origin_);
    if (distance > MAX_DISTANCE) {
        distance = MAX_DISTANCE - 0.01f;
    }
    
    float strength = (MAX_DISTANCE - distance) / MAX_DISTANCE;
    float force = strength * MAX_FORCE;
    float angle = atan2f(bodyPos.y - origin_.y, bodyPos.x - origin_.x);

    // Apply an impulse to the body, using the angle.
    body->ApplyLinearImpulse(b2Vec2(cosf(angle) * force, sinf(angle) * force),
                             body->GetPosition());

    return true;
}
