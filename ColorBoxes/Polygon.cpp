//
//  Polygon.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 1/15/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#include "Polygon.h"
#include "Utilities.h"
#include "GLColor.h"
#include "ColorBoxesEngine.h"


Polygon::Polygon(float x, float y, int numVertices, ColorBoxesEngine* engine)
    : PhysicsEntity(numVertices, engine),
      radius_(randomFloat(2.0f, 16.0f))
{
    borderColor_ = GLColor::randomRGBColor();
    fillColor_ = borderColor_.lighten(0.5f);
    makeBody(b2Vec2(x, y));
}

Polygon::~Polygon()
{
}

float
Polygon::boundingRadius() const
{
    return radius_;
}

b2Shape*
Polygon::makeShape()
{
    float worldRadius = engine_->scalarPixelsToWorld(radius_);
    
    b2Vec2* vertices = new b2Vec2[numVertices_];
    for (int i = 0; i < numVertices_; ++i) {
        vertices[i].x = worldRadius * cos(2.0f * M_PI * i / numVertices_);
        vertices[i].y = worldRadius * sin(2.0f * M_PI * i / numVertices_);
    }
    
    b2PolygonShape* shape = new b2PolygonShape;
    shape->Set(vertices, numVertices_);
    
    delete [] vertices;
    
    return shape;
}