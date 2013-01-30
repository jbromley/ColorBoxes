//
//  OpenGLDraw.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 1/17/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#include "OpenGLDraw.h"
#include "Utilities.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "GL/freeglut.h"
#endif


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
    }
    
    void drawSolidCircleAxis(const b2Vec2& center, float radius, const b2Vec2& axis,
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
    
    void drawStringTTF(const std::string &text, float x, float y, TTF_Font* font, const GLColor& color)
    {
        // Use SDL_TTF to render the text onto an initial surface.
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color.toSDLColor());
        
        // Convert the rendered text to a known format.
        int w = nextPowerOfTwo(textSurface->w);
        int h = nextPowerOfTwo(textSurface->h);
        
        SDL_Surface* intermediary = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, w, h, 32,
                                                         0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        SDL_SetAlpha(textSurface, 0, 0);
        SDL_BlitSurface(textSurface, NULL, intermediary, NULL);
        
        // Tell GL about our new texture.
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA,
                     GL_UNSIGNED_BYTE, intermediary->pixels );
        
        // GL_NEAREST looks horrible if scaled.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Prepare to render our texture.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glColor4f(color.r, color.g, color.b, color.a);
        
        // Draw a quad at location.
        glBegin(GL_QUADS);
        // Recall that the origin is in the lower-left corner. That is why the
        // TexCoords specify different corners than the Vertex coors seem to.
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(x + w, y);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(x + w, y + h);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(x, y + h);
        glEnd();
        
        // Bad things happen if we delete the texture before it finishes.
        glFinish();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        
        // Clean up.
        SDL_FreeSurface(intermediary);
        SDL_FreeSurface(textSurface);
        glDeleteTextures(1, &texture);
    }

    
}
