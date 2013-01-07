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
    textColor_(GLColor::white())
{
    // Set up Box2D world.
    b2Vec2 gravity(0.0f, -10.0f);
    bool doSleep = true;
#ifdef __APPLE__
    world_ = new b2World(gravity, doSleep);
#else
    world_ = new b2World(gravity);
    world_->SetAllowSleeping(doSleep);
#endif
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

    walls_.push_back(new Wall(0.0f, 0.0f,
                              5.0f, height() - 20.0f, this));
    walls_.push_back(new Wall(width() - 6.0f, 0.0f,
                              width() - 1.0f, height() - 20.0f, this));
    walls_.push_back(new Wall(width() / 16.0f, height() - 15.0f,
                              6.0f * width() / 16.0f, 5.0, this));
    walls_.push_back(new Wall(9.0f * width() / 16.0f, height() - 15.0f,
                              6.0f * width() / 16.0f, 5.0, this));
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
        boxes_.push_back(new Box(x, y, this));
    }
    
    // Clear out any boxes that are off the screen.
    for_each(boxes_.begin(), boxes_.end(), deleteDoneBoxes);
    boxes_.erase(std::remove(boxes_.begin(), boxes_.end(), static_cast<Box*>(0)), boxes_.end());
    
    world_->Step(timeStep, velocityIterations, positionIterations);
}

void
ColorBoxesEngine::render()
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
    renderText(msg.str(), 8.0f, 2.0f);
}

void
ColorBoxesEngine::renderText(const std::string &text, float x, float y)
{
	// Use SDL_TTF to render the text onto an initial surface.
	SDL_Surface* textSurface = TTF_RenderText_Blended(font_, text.c_str(), textColor_.toSDLColor());
	
	/* Convert the rendered text to a known format */
	int w = nextPowerOfTwo(textSurface->w);
	int h = nextPowerOfTwo(textSurface->h);
	
	SDL_Surface* intermediary = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, w, h, 32,
                                                     0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_SetAlpha(textSurface, 0, 0);
	SDL_BlitSurface(textSurface, NULL, intermediary, NULL);
	
	/* Tell GL about our new texture */
    GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, intermediary->pixels );
	
	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
	/* prepare to render our texture */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor4f(textColor_.r, textColor_.g, textColor_.b, textColor_.a);
	
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

void
ColorBoxesEngine::end()
{
}

void
ColorBoxesEngine::keyDown(int keyCode)
{
    switch (keyCode) {
        case SDLK_b:
            if (backgroundColor() == GLColor::black()) {
                setBackgroundColor(GLColor::white());
                textColor_ = GLColor::black();
            } else {
                setBackgroundColor(GLColor::black());
                textColor_ = GLColor::white();
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

float
ColorBoxesEngine::scaleFactor() const
{
    return scaleFactor_;
}


