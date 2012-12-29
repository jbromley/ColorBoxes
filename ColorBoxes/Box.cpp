//
//  Box.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 12/27/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#include <utility>
#include <SDL/SDL_gfxPrimitives.h>
#include "Box.h"
#include "Utilities.h"
#include "ColorBoxesEngine.h"


Box::Box(float x, float y, ColorBoxesEngine* engine)
: w_(randomFloat(2.0f, 16.0f)),
  h_(randomFloat(4.0f, 16.0f)),
  engine_(engine)
{
    borderColor_ = GLColor::randomRGBColor();
    fillColor_ = borderColor_.lighten(0.5f);
    makeBody(b2Vec2(x, y), w_, h_);
}

Box::~Box()
{
    body_->GetWorld()->DestroyBody(body_);
}

void
Box::update(long timeElapsed)
{
}

void
Box::render()
{
    b2Vec2 worldCenter = body_->GetPosition();
    float32 theta = body_->GetAngle();
    b2Transform t;
    t.Set(worldCenter, theta);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(fillColor_.r, fillColor_.g, fillColor_.b, fillColor_.a);
    
    // Position for drawing
//    glPushMatrix();
//    float scale = engine_->scaleFactor();
//    glScalef(scale, scale, scale);
//    b2Vec2 worldPos = body_->GetPosition();
//    glTranslatef(worldPos.x, worldPos.y, 0.0f);
//    double theta = body_->GetAngle();
//    glRotated(theta * 180.0f / M_PI, 0.0, 0.0, 1.0);
    
    // Draw the vertices.
    const b2Fixture* fixture = body_->GetFixtureList();
    const b2PolygonShape* polygon = dynamic_cast<const b2PolygonShape*>(fixture->GetShape());

    b2Vec2 vertices[4];
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < polygon->m_vertexCount; ++i) {
        vertices[i] = engine_->coordWorldToPixels(b2Mul(t, polygon->m_vertices[i]));
        glVertex2f(vertices[i].x, vertices[i].y);
    }
    glEnd();
    glDisable(GL_BLEND);
    
    glColor4f(borderColor_.r, borderColor_.g, borderColor_.b, borderColor_.a);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < polygon->m_vertexCount; ++i) {
        glVertex2f(vertices[i].x, vertices[i].y);
    }
    glEnd();
//    glPopMatrix();
    
//    SDL_Surface* surface  = engine_->surface();
//    
//    b2Vec2 worldCenter = body_->GetPosition();
//    float32 theta = body_->GetAngle();
//    b2Transform t;
//    t.Set(worldCenter, theta);
//    
//    Sint16 vx[4];
//    Sint16 vy[4];
//    
//    // Get the vertices so we can transform them.
//    const b2Fixture* fixture = body_->GetFixtureList();
//    const b2PolygonShape* polygon = dynamic_cast<const b2PolygonShape*>(fixture->GetShape());
//    for (int i = 0; i < polygon->m_vertexCount; ++i) {
//        b2Vec2 v = engine_->coordWorldToPixels(b2Mul(t, polygon->m_vertices[i]));
//        vx[i] = roundToInt(v.x);
//        vy[i] = roundToInt(v.y);
//    }
//    
//    aapolygonColor(surface, vx, vy, 4, borderColor_);
//    filledPolygonColor(surface, vx, vy, 4, fillColor_);
}

bool
Box::done()
{
    b2Vec2 worldPos = body_->GetPosition();
    b2Vec2 pixelPos = engine_->coordWorldToPixels(worldPos);
    
    if (pixelPos.y > engine_->height() + w_ * h_) {
        return true;
    }
    
    return false;
}

void
Box::makeBody(const b2Vec2& center, float width, float height)
{
    b2World* world = engine_->world();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = engine_->coordPixelsToWorld(center);
    body_ = world->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    box.SetAsBox(engine_->scalarPixelsToWorld(width),
                 engine_->scalarPixelsToWorld(height));
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1.0;
    fixtureDef.friction = 0.3;
    fixtureDef.restitution = 0.5;
    body_->CreateFixture(&fixtureDef);
    body_->SetUserData(this);
    
    // Give it some initial random velocity.
    body_->SetLinearVelocity(b2Vec2(randomFloat(-5.0f, 5.0f),
                                    randomFloat(2.0f, 5.0f)));
    body_->SetAngularVelocity(randomFloat(-M_2_PI, M_2_PI));
}

void
Box::killBody()
{
    b2World* world = engine_->world();
    world->DestroyBody(body_);
}