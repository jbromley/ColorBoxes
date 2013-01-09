//
//  Edge.h
//  ColorBoxes
//
//  Created by Jay Bromley on 01/07/2013
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__Edge__
#define __ColorBoxes__Edge__

#include <iostream>
#include <Box2D/Box2D.h>
#include "GLColor.h"


class ColorBoxesEngine;

class Edge
{
public:
    Edge(const b2Vec2& pt1, const b2Vec2& pt2, const GLColor& color, ColorBoxesEngine* engine);
    ~Edge();

    void setEndPoint(const b2Vec2& endPt);
    void setColor(const GLColor& color);
    void render();
    
private:
    void makeBody(const b2Vec2& pt1, const b2Vec2&pt2);

    b2Vec2 startPt_;
    b2Vec2 endPt_;
    b2EdgeShape shape_;
    b2Body* body_;
    GLColor color_;
    ColorBoxesEngine* engine_;
};

#endif /* defined(__ColorBoxes__Edge__) */
