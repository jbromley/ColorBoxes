//
//  Circle.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 1/15/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#include "Circle.h"
#include "Utilities.h"
#include "OpenGLDraw.h"
#include "GLColor.h"
#include "ColorBoxesEngine.h"


Circle::Circle(float x, float y, ColorBoxesEngine* engine)
: Shape(0, engine),
    radius_(randomFloat(2.0f, 16.0f))
{
    borderColor_ = GLColor::randomRGBColor();
    fillColor_ = borderColor_.lighten(0.5f);
    makeBody(b2Vec2(x, y));
}

Circle::~Circle()
{
}

void
Circle::render()
{
    b2Vec2 center = engine_->coordWorldToPixels(body_->GetPosition());
    b2Vec2 axis(cosf(body_->GetAngle()), sinf(body_->GetAngle()));
    
    ogl::drawSolidCircle(center, radius_, axis, borderColor_, fillColor_);
}

float
Circle::shapeHeight() const
{
    return 2.0f * radius_;
}

b2Shape*
Circle::makeShape()
{
    float worldRadius = engine_->scalarPixelsToWorld(radius_);
    b2CircleShape* shape = new b2CircleShape;
    shape->m_radius = worldRadius;
    return shape;
}