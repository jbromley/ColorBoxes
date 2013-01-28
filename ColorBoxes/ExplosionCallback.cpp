//
//  ExplosionCallback.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 1/24/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#include "ExplosionCallback.h"

ExplosionCallback::ExplosionCallback(const b2Vec2& origin, float blastRadius, float impulse)
: origin_(origin),
  blastRadius_(blastRadius),
  impulse_(impulse)
{
}

ExplosionCallback::~ExplosionCallback()
{
}

bool
ExplosionCallback::ReportFixture(b2Fixture* fixture)
{
    b2Body* body = fixture->GetBody();
    b2Vec2 bodyPos = body->GetPosition();
    float distance = b2Distance(bodyPos, origin_);
    if (distance == 0.0f) {
        return true;
    }
    
    // Linear fall-off explosion
//    if (distance > blastRadius_) {
//        distance = blastRadius_ - 0.01f;
//    }
//    
//    float force = impulse_ * (blastRadius_ - distance) / blastRadius_;
    
    // 1/r^2 fall-off explosion.
    float force = 4.0f * impulse_ / (distance * distance);
    b2Vec2 hitVector(bodyPos);
    hitVector -= origin_;
    hitVector.Normalize();
    
    std::cout << force << std::endl;
    
    // Apply an impulse to the body, using the angle.
    body->ApplyLinearImpulse(force * hitVector, body->GetPosition());

    return true;
}
