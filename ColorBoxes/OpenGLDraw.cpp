//
//  OpenGLDraw.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 1/17/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#include "OpenGLDraw.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "freeglut/freeglut.h"
#endif
#include <SDL/SDL.h>


namespace ogl
{
    void drawSolidPolygon(const b2Vec2* vertices, int vertexCount,
                          const GLColor& borderColor, const GLColor& fillColor)
    {
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
        glBegin(GL_TRIANGLE_FAN);
        for (int32 i = 0; i < vertexCount; ++i) {
            glVertex2f(vertices[i].x, vertices[i].y);
        }
        glEnd();
        glDisable(GL_BLEND);
        
        glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        glBegin(GL_LINE_LOOP);
        for (int32 i = 0; i < vertexCount; ++i) {
            glVertex2f(vertices[i].x, vertices[i].y);
        }
        glEnd();
    }

    void drawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis,
                         const GLColor& borderColor, const GLColor& fillColor)
    {
        const float32 SEGMENTS = 16.0f;
        const float32 INCREMENT = 2.0f * b2_pi / SEGMENTS;
        
        float32 theta = 0.0f;
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
        glBegin(GL_TRIANGLE_FAN);
        for (int32 i = 0; i < SEGMENTS; ++i) {
            b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
            glVertex2f(v.x, v.y);
            theta += INCREMENT;
        }
        glEnd();
        glDisable(GL_BLEND);
        
        theta = 0.0f;
        glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        glBegin(GL_LINE_LOOP);
        for (int32 i = 0; i < SEGMENTS; ++i)  {
            b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
            glVertex2f(v.x, v.y);
            theta += INCREMENT;
        }
        glEnd();
        
        b2Vec2 p = center + radius * axis;
        glBegin(GL_LINES);
        glVertex2f(center.x, center.y);
        glVertex2f(p.x, p.y);
        glEnd();
    }
    
    void drawSegment(const b2Vec2& p1, const b2Vec2& p2, float thickness, const GLColor& color)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glColor4f(color.r, color.g, color.b, color.a);
        glLineWidth(thickness);
        glBegin(GL_LINES);
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
        glEnd();

        glLineWidth(1.0f);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }
    
    void drawPoint(const b2Vec2& p, float32 size, const GLColor& color)
    {
        glPointSize(size);
        glBegin(GL_POINTS);
        glColor4f(color.r, color.g, color.b, color.a);
        glVertex2f(p.x, p.y);
        glEnd();
        glPointSize(1.0f);
    }
    
    void drawString(int x, int y, const std::string& text, const GLColor& color)
    {
        const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, videoInfo->current_w, videoInfo->current_h, 0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glColor4f(color.r, color.g, color.b, color.a);
        glRasterPos2i(x, y);
        size_t length = text.size();
        for (size_t i = 0; i < length; ++i) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
        }
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
    
}