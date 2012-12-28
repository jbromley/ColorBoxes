//
//  Wall.h
//  ColorBoxes
//
//  Created by Jay Bromley on 12/27/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__Wall__
#define __ColorBoxes__Wall__

#include <iostream>

class b2Body;
class ColorBoxesEngine;

class Wall
{
public:
    Wall(float x, float y, float width, float height, ColorBoxesEngine* engine);
    ~Wall();
    
    void render();
    
private:
    float x_;
    float y_;
    float w_;
    float h_;
    
    Uint32 borderColor_;
    Uint32 fillColor_;
    
    b2Body* body_;
    
    ColorBoxesEngine* engine_;
};

#endif /* defined(__ColorBoxes__Wall__) */
