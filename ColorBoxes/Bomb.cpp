//
//  Bomb.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 1/27/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#include "Bomb.h"
#include "ExplosionCallback.h"
#include "OpenGLDraw.h"
#include "GLColor.h"
#include "ColorBoxesEngine.h"
#include <sstream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "GL/freeglut.h"
#endif


Bomb::Bomb(float x, float y, float blastRadius, float strength,
           float fuseLength, ColorBoxesEngine* engine)
    : PhysicsEntity(0, engine),
      radius_(12.0f),
      blastRadius_(blastRadius),
      maxImpulse_(strength),
      countdown_(fuseLength)
{
    borderColor_ = GLColor::red();
    fillColor_ = borderColor_.lighten(0.8f);
    makeBody(b2Vec2(x, y));
}

Bomb::~Bomb()
{
}

void
Bomb::update(long millisElapsed)
{
    countdown_ -= (millisElapsed / 1000.0f);
    if (countdown_ < 0.0f) {
        explode();
    }
}

void
Bomb::render()
{
    b2Vec2 center = engine_->coordWorldToPixels(body_->GetPosition());
    b2Vec2 axis(cosf(body_->GetAngle()), sinf(body_->GetAngle()));
    
    std::ostringstream s;
    s << static_cast<int>(ceilf(countdown_));
    
    int w = 0;
    int h = 0;
    TTF_SizeText(engine_->font(), s.str().c_str(), &w, &h);
    ogl::drawSolidCircle(center, radius_, axis, borderColor_, fillColor_);
//    ogl::drawString(center.x - w / 2.0f, center.y - h / 2.0f, s.str(), GLColor::white());
    ogl::drawStringTTF(s.str(), center.x - w / 2.0f, center.y - h / 2.0f,
                       engine_->font(), GLColor::white());
                    
}

bool
Bomb::done() const
{
    return countdown_ < 0.0f;
}

float
Bomb::boundingRadius() const
{
    return 2.0f * radius_;
}

b2Shape*
Bomb::makeShape()
{
    float worldRadius = engine_->scalarPixelsToWorld(radius_);
    b2CircleShape* shape = new b2CircleShape;
    shape->m_radius = worldRadius;
    return shape;
}

void
Bomb::explode()
{
    b2Vec2 worldPos = body_->GetPosition();
    
    // Create the query AABB.
    b2Vec2 diagonal(blastRadius_, blastRadius_);
    b2Vec2 lowerBound = worldPos;
    lowerBound -= diagonal;
    b2Vec2 upperBound = worldPos;
    upperBound += diagonal;
    
    b2AABB bounds = {lowerBound, upperBound};
    
    // Set the query callback.
    ExplosionCallback explosion(worldPos, blastRadius_, maxImpulse_);
    engine_->world()->QueryAABB(&explosion, bounds);
}


