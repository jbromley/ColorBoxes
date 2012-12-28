//
//  Box.h
//  ColorBoxes
//
//  Created by Jay Bromley on 12/27/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__Box__
#define __ColorBoxes__Box__

#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL/SDL.h>


class ColorBoxesEngine;

class Box
{
public:
    Box(float x, float y, ColorBoxesEngine* engine);
    ~Box();
    
    void update(long timeElapsed);
    void render();
    bool done();
    
private:
    void makeBody(const b2Vec2& center, float width, float height);
    void killBody();
    
    float w_;
    float h_;
    
    b2Body* body_;
    
    Uint32 fillColor_;
    Uint32 borderColor_;
    
    ColorBoxesEngine* engine_;
};

#endif /* defined(__ColorBoxes__Box__) */
