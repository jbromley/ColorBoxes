//
//  ColorBoxesEngine.cpp
//  ColorBoxes
//
//  Created by Jay Bromley on 12/27/12.
//  Copyright (c) 2012 Jay Bromley. All rights reserved.
//

#include <algorithm>
#include <functional>
#include <sstream>
#include "ColorBoxesEngine.h"
#include "Utilities.h"
#include "Wall.h"
#include "Box.h"


void deleteDoneBoxes(Box*& box)
{
    if (box->done()) {
        delete box;
        box = 0;
    }
}

ColorBoxesEngine* ColorBoxesEngine::self = NULL;

ColorBoxesEngine*
ColorBoxesEngine::instance()
{
    return ColorBoxesEngine::self;
}

ColorBoxesEngine::ColorBoxesEngine(int w, int h, const char* resourcePath)
: GameEngine(w, h, resourcePath),
    createBoxes_(false),
    renderStats_(false),
    scaleFactor_(10.0f),
    yFlip_(-1.0f),
    textColor_({255, 255, 255})
{
    // Set up Box2D world.
    b2Vec2 gravity(0.0f, -10.0f);
    bool doSleep = true;
    world_ = new b2World(gravity, doSleep);
    world_->SetWarmStarting(true);
    world_->SetContinuousPhysics(true);
    
    // Set up SDL TTF.
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf error: " << TTF_GetError() << std::endl;
    }
    
    std::string fontPath = std::string(resourcePath) + "/Arial.ttf";
    font_ = TTF_OpenFont(fontPath.c_str(), 16);
    if (font_ == NULL) {
        std::cerr << "SDL_ttf error: " << TTF_GetError() << std::endl;
    } else {
        TTF_SetFontStyle(font_, TTF_STYLE_BOLD);
    }
    
    ColorBoxesEngine::self = this;
}

ColorBoxesEngine::~ColorBoxesEngine()
{
    delete world_;

    TTF_CloseFont(font_);
    TTF_Quit();
    SDL_Quit();
}

void
ColorBoxesEngine::initializeData()
{
    setTitle("Box Fall");

    walls_.push_back(new Wall(0.0f, 0.0f, 5.0f, height() - 20.0f));
    walls_.push_back(new Wall(width() - 6.0f, 0.0f, width() - 1.0f, height() - 20.0f));
    walls_.push_back(new Wall(width() / 16.0f, height() - 15.0f, 6.0f * width() / 16.0f, 5.0));
    walls_.push_back(new Wall(9.0f * width() / 16.0f, height() - 15.0f, 6.0f * width() / 16.0f, 5.0));
    
}

void
ColorBoxesEngine::update(long elapsedTime)
{
    const int32 velocityIterations = 6;
    const int32 positionIterations = 2;
    float32 timeStep = elapsedTime / 1000.0f;
    
    // Create a box if we are in create mode.
    if (createBoxes_) {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);
        boxes_.push_back(new Box(x, y));
    }
    
    // Clear out any boxes that are off the screen.
    for_each(boxes_.begin(), boxes_.end(), deleteDoneBoxes);
    boxes_.erase(std::remove(boxes_.begin(), boxes_.end(), static_cast<Box*>(0)), boxes_.end());
    
    world_->Step(timeStep, velocityIterations, positionIterations);
}

void
ColorBoxesEngine::render(SDL_Surface* destSurface)
{
    for_each(walls_.begin(), walls_.end(), std::mem_fun(&Wall::render));
    for_each(boxes_.begin(), boxes_.end(), std::mem_fun(&Box::render));
    
    if (renderStats_) {
        renderStatistics();
    }
}

void
ColorBoxesEngine::renderStatistics()
{
    std::ostringstream msg;
    msg << boxes_.size() << " boxes, " << fps() << " fps";
    SDL_Surface* textSurface = TTF_RenderText_Blended(font_, msg.str().c_str(), textColor_);
    SDL_Rect destRect = {8, 2, 0, 0};
    SDL_BlitSurface(textSurface, NULL, surface(), &destRect);
    SDL_FreeSurface(textSurface);
}

void
ColorBoxesEngine::end()
{
}

void
ColorBoxesEngine::keyDown(int keyCode)
{
    switch (keyCode) {
        case SDLK_b:
            if (backgroundColor() == 0x000000ff) {
                setBackgroundColor(0xffffffff);
                textColor_ = {0, 0, 0};
            } else {
                setBackgroundColor(0x000000ff);
                textColor_ = {255, 255, 255};
            }
            break;
        case SDLK_s:
            renderStats_ = !renderStats_;
            std::cout << "render statistics = " << (renderStats_ ? "true" : "false") << std::endl;
            break;
        default:
            std::cout << "keyDown(" << keyCode << ")" << std::endl;
            break;
    }
}

//void
//ColorBoxesEngine::keyUp(int keyCode)
//{
//    
//}

void
ColorBoxesEngine::mouseButtonDown(int button, int x, int y, int dx, int dy)
{
    if (button == SDL_BUTTON_LEFT) {
        createBoxes_ = true;
    }
}

void
ColorBoxesEngine::mouseButtonUp(int button, int x, int y, int dx, int dy)
{
    if (button == SDL_BUTTON_LEFT) {
        createBoxes_ = false;
    }
}

b2World*
ColorBoxesEngine::world()
{
    return world_;
}

b2Vec2
ColorBoxesEngine::coordWorldToPixels(float worldX, float worldY)
{
    float pixelX = worldX * scaleFactor_;
    float pixelY = worldY * scaleFactor_;
    if (yFlip_ == -1.0f) {
        pixelY = map(pixelY, 0.0f, static_cast<float>(height()),
                     static_cast<float>(height()), 0.0f);
    }
    return b2Vec2(pixelX, pixelY);
}

b2Vec2
ColorBoxesEngine::coordWorldToPixels(const b2Vec2& v)
{
    return coordWorldToPixels(v.x, v.y);
}


b2Vec2
ColorBoxesEngine::coordPixelsToWorld(float pixelX, float pixelY)
{
    float worldX = pixelX / scaleFactor_;
    float worldY = pixelY;
    if (yFlip_ == -1.0f) {
        worldY = map(pixelY, static_cast<float>(height()), 0.0f,
                     0.0f, static_cast<float>(height()));
    }
    worldY /= scaleFactor_;
    return b2Vec2(worldX, worldY);
}

b2Vec2
ColorBoxesEngine::coordPixelsToWorld(const b2Vec2& v)
{
    return coordPixelsToWorld(v.x, v.y);
}


float
ColorBoxesEngine::scalarWorldToPixels(float val)
{
    return val * scaleFactor_;
}

float
ColorBoxesEngine::scalarPixelsToWorld(float val)
{
    return val / scaleFactor_;
}


