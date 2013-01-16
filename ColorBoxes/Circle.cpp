//
//  Circle.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 1/15/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#include "Circle.h"
#include "Utilities.h"
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
    const int segments = 16;
    const float increment = 2.0f * b2_pi / segments;
    
    b2Vec2 center = engine_->coordWorldToPixels(body_->GetPosition());
    float32 theta = body_->GetAngle();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    
    float t = 0.0f;
    b2Vec2 vertices[segments];
    glColor4f(fillColor_.r, fillColor_.g, fillColor_.b, fillColor_.a);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < segments; ++i) {
        vertices[i] = center + radius_ * b2Vec2(cosf(t), sinf(t));
		glVertex2f(vertices[i].x, vertices[i].y);
		t += increment;
    }
    glEnd();
    
    t = 0.0f;
    glColor4f(borderColor_.r, borderColor_.g, borderColor_.b, borderColor_.a);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        glVertex2f(vertices[i].x, vertices[i].y);
        t += increment;
    }
    glEnd();
    
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);

    glBegin(GL_LINES);
    glVertex2f(center.x, center.y);
    glVertex2f(center.x + radius_ * cosf(theta),
               center.y + radius_ * sinf(theta));
    glEnd();
    
    glDisable(GL_LINE_SMOOTH);
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