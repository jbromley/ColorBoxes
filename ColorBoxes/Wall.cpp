//
//  Wall.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 12/27/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#include <Box2D/Box2D.h>
#include "Wall.h"
#include "ColorBoxesEngine.h"
#include "Utilities.h"


/**
 * Creates a new wall.
 * @param x the x-coordinate of the wall's center in pixels
 * @param y the y-coordinate of the wall's center in pixels
 * @param w the wall's width in pixels
 * @param h the wall's width in pixels
 */
Wall::Wall(float x, float y, float w, float h, ColorBoxesEngine* engine)
: x_(x), y_(y), w_(w), h_(h), engine_(engine)
{
    b2World* world = engine_->world();
    
    // Set up for Box2D.
    b2Vec2 center = engine_->coordPixelsToWorld(x + w / 2, y + h / 2);
    float horizExtent = engine_->scalarPixelsToWorld(w / 2.0f);
    float vertExtent = engine_->scalarPixelsToWorld(h / 2.0f);
    
    b2BodyDef bodyDef;
    bodyDef.position = center;
    body_ = world->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    box.SetAsBox(horizExtent, vertExtent);
    
    body_->CreateFixture(&box, 0.0f);
    body_->SetUserData(this);
    
    // Set up colors.
    borderColor_ = GLColor::magenta();
    fillColor_ = borderColor_.lighten(0.5f);
}

Wall::~Wall()
{
    body_->GetWorld()->DestroyBody(body_);
}

void
Wall::render()
{
    float x2 = x_ + w_ - 1;
    float y2 = y_ + h_ - 1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(fillColor_.r, fillColor_.g, fillColor_.b, fillColor_.a);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x_, y_);
    glVertex2f(x_, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y_);
    glEnd();
    glDisable(GL_BLEND);
    
    glColor4f(borderColor_.r, borderColor_.g, borderColor_.b, borderColor_.a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x_, y_);
    glVertex2f(x_, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y_);
    glEnd();
}
