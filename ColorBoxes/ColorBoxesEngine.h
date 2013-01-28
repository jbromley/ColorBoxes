//
//  ColorBoxesEngine.h
//  ColorBoxes
//
//  Created by Jay Bromley on 12/27/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#ifndef __ColorBoxes__ColorBoxesEngine__
#define __ColorBoxes__ColorBoxesEngine__

#include <iostream>
#include <string>
#include <vector>
#include <Box2D/Box2D.h>
#include <SDL/SDL_ttf.h>

#include "GameEngine.h"


class Wall;
class PhysicsEntity;
class Box;
class Edge;

class ColorBoxesEngine: public GameEngine
{
public:
    static ColorBoxesEngine* instance();
    ColorBoxesEngine(int w, int h, const char* resourcePath = "");
    virtual ~ColorBoxesEngine();
    
    virtual void initializeData();
    virtual void update(long elapsedTime);
    virtual void render();
    virtual void end();
    
    virtual void keyDown(int keyCode);
//    virtual void keyUp(int keyCode);
    virtual void mouseButtonDown(int button, int x, int y, int dx, int dy);
    virtual void mouseButtonUp(int button, int x, int y, int dx, int dy);
    
    unsigned long objectCount() const;
    void resetWorld();

    b2World* world();
    b2Vec2 coordWorldToPixels(const b2Vec2& v);
    b2Vec2 coordWorldToPixels(float worldX, float worldY);
    b2Vec2 coordPixelsToWorld(const b2Vec2& v);
    b2Vec2 coordPixelsToWorld(float pixelX, float pixelY);
    float scalarWorldToPixels(float val);
    float scalarPixelsToWorld(float val);
    
    float scaleFactor() const;
    
private:
    enum ObjectShape {
        TRIANGLE,
        QUADRILATERAL,
        PENTAGON,
        HEXAGON,
        HEPTAGON,
        OCTAGON,
        CIRCLE,
        BOX,
        NUMBER_OBJECT_SHAPES
    };
    
    enum State {
        NORMAL,
        CREATE_OBJECT,
        CREATE_EDGE,
        DELETE_EDGE,
        CREATE_BOMB,
        NUMBER_STATES
    };
    
    static const float SELECTION_THRESHOLD;
    static const GLColor SELECTION_COLOR;
    static const GLColor EDGE_COLOR;
    static ColorBoxesEngine* self;

    void configureTweakBar();
    void renderStatistics();
    void renderText(const std::string& text, float x, float y);
    
    std::vector<Wall*> walls_;
    std::vector<PhysicsEntity*> objects_;
    std::vector<Edge*> edges_;
    
    State state_;
    ObjectShape currentShape_;
    Edge* newEdge_;
    Edge* selectedEdge_;
    bool renderStats_;
    
    b2World* world_;
    b2Vec2 gravity_;
    float scaleFactor_;
    float yFlip_;
    
    TTF_Font* font_;
    GLColor textColor_;
    
    SDL_Cursor* defaultCursor_;
};


#endif /* defined(__ColorBoxes__ColorBoxesEngine__) */
