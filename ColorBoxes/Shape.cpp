//
//  Shape.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 1/14/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#include "Shape.h"
#include "Utilities.h"
#include "OpenGLDraw.h"
#include "ColorBoxesEngine.h"


Shape::Shape(int numVertices, ColorBoxesEngine* engine)
: numVertices_(numVertices),
    borderColor_(GLColor::randomColor()),
    fillColor_(borderColor_.lighten(0.5f)),
    engine_(engine)
{
    
}

Shape::~Shape()
{
    body_->GetWorld()->DestroyBody(body_);
}

void
Shape::update(long timeElapsed)
{
    
}

void
Shape::render()
{
    const int MAX_VERTICES = 16;
    
    b2Vec2 worldCenter = body_->GetPosition();
    float32 theta = body_->GetAngle();
    b2Transform t;
    t.Set(worldCenter, theta);
    
    // Calculate the vertices.
    const b2Fixture* fixture = body_->GetFixtureList();
    const b2PolygonShape* polygon = dynamic_cast<const b2PolygonShape*>(fixture->GetShape());
    
    b2Vec2 vertices[MAX_VERTICES];
    for (int i = 0; i < polygon->m_vertexCount; ++i) {
        vertices[i] = engine_->coordWorldToPixels(b2Mul(t, polygon->m_vertices[i]));
    }

    // Draw.
    ogl::drawSolidPolygon(vertices, polygon->m_vertexCount, borderColor_, fillColor_);
}

bool
Shape::done() const
{
    b2Vec2 worldPos = body_->GetPosition();
    b2Vec2 pixelPos = engine_->coordWorldToPixels(worldPos);
    
    if (pixelPos.y > engine_->height() + shapeHeight()) {
        return true;
    }
    
    return false;
}

void
Shape::makeBody(const b2Vec2 &center)
{
    b2World* world = engine_->world();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = engine_->coordPixelsToWorld(center);
    body_ = world->CreateBody(&bodyDef);
    
    b2Shape* shape = makeShape();    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = shape;
    fixtureDef.density = 1.0;
    fixtureDef.friction = 0.3;
    fixtureDef.restitution = 0.5;
    body_->CreateFixture(&fixtureDef);
    body_->SetUserData(this);
    
    // Give it some initial random velocity.
    body_->SetLinearVelocity(b2Vec2(randomFloat(-5.0f, 5.0f),
                                    randomFloat(2.0f, 5.0f)));
    body_->SetAngularVelocity(randomFloat(-M_2_PI, M_2_PI));
    
    delete shape;
}

void
Shape::killBody()
{
    b2World* world = engine_->world();
    world->DestroyBody(body_);
}