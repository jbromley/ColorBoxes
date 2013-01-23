//
//  Edge.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 01/07/2013
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//
#include "Edge.h"
#include "ColorBoxesEngine.h"
#include "OpenGLDraw.h"


Edge::Edge(const b2Vec2& pt1, const b2Vec2& pt2, const GLColor& color, ColorBoxesEngine* engine)
    : startPt_(pt1), 
      endPt_(pt2),
      body_(NULL),
      color_(color),
      engine_(engine)
{
    makeBody(pt1, pt2);
}

Edge::~Edge()
{
    body_->GetWorld()->DestroyBody(body_);
}
    
void
Edge::setEndPoint(const b2Vec2& endPoint)
{
    endPt_ = endPoint;
    engine_->world()->DestroyBody(body_);
    makeBody(startPt_, endPt_);
}

void
Edge::setColor(const GLColor& color)
{
    color_ = color;
}

void
Edge::makeBody(const b2Vec2& pt1, const b2Vec2& pt2)
{
    b2Vec2 wpt1 = engine_->coordPixelsToWorld(pt1);
    b2Vec2 wpt2 = engine_->coordPixelsToWorld(pt2);

    b2BodyDef edgeBodyDef;
    body_ = engine_->world()->CreateBody(&edgeBodyDef);
    shape_.Set(wpt1, wpt2);
    b2FixtureDef edgeFixtureDef;
    edgeFixtureDef.density = 1.0;
    edgeFixtureDef.friction = 0.0;
    edgeFixtureDef.shape = &shape_;
    body_->CreateFixture(&edgeFixtureDef);
}

void Edge::render()
{
    ogl::drawSegment(startPt_, endPt_, 3.0f, color_);
}

float pointToEdgeDistance(const b2Vec2& p, const Edge* edge)
{
    b2Vec2 ba(edge->endPt_);
    ba -= edge->startPt_;
    
    b2Vec2 pa(p);
    pa -= edge->startPt_;

    b2Vec2 pb(p);
    pb -= edge->endPt_;
    
    float distance = b2Cross(ba, pa) / ba.Length();
    if (b2Dot(pb, ba) > 0.0) {
        distance = pb.Length();
    } else if (b2Dot(pa, ba) < 0.0) {
        distance = pa.Length();
    }
    return fabsf(distance);
}
