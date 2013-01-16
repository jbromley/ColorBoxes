//
//  Box.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 12/27/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#include <utility>
#include "Box.h"
#include "Utilities.h"
#include "ColorBoxesEngine.h"


Box::Box(float x, float y, ColorBoxesEngine* engine)
: Shape(4, engine),
  w_(randomFloat(2.0f, 16.0f)),
  h_(randomFloat(4.0f, 16.0f))
{
    borderColor_ = GLColor::randomRGBColor();
    fillColor_ = borderColor_.lighten(0.5f);
    makeBody(b2Vec2(x, y));
}

Box::~Box()
{
}

float
Box::shapeHeight() const
{
    return w_ * h_;
}

b2Shape*
Box::makeShape()
{
    b2PolygonShape* boxShape = new b2PolygonShape;
    boxShape->SetAsBox(engine_->scalarPixelsToWorld(w_),
                       engine_->scalarPixelsToWorld(h_));
    return boxShape;
}

