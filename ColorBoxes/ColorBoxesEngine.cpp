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
#include <AntTweakBar.h>
#include "ColorBoxesEngine.h"
#include "OpenGLDraw.h"
#include "Utilities.h"
#include "Wall.h"
#include "Circle.h"
#include "Polygon.h"
#include "Box.h"
#include "Edge.h"
#include "ExplosionCallback.h"
#include "Cursors.h"


// AntTweakBar helpers

static void TW_CALL
getObjectCount(void* value, void* clientData)
{
    *static_cast<unsigned long*>(value) = static_cast<const ColorBoxesEngine*>(clientData)->objectCount();
}

static void TW_CALL
getFps(void* value, void* clientData)
{
    *static_cast<int*>(value) = static_cast<const ColorBoxesEngine*>(clientData)->fps();
}

static void TW_CALL
getGravityStrength(void* value, void* clientData)
{
    b2Vec2 gravity = static_cast<ColorBoxesEngine*>(clientData)->world()->GetGravity();
    *static_cast<float*>(value) = gravity.Length();
}

static void TW_CALL
setGravityStrength(const void* value, void* clientData)
{
    float strength = *static_cast<const float*>(value);
    b2Vec2 gravity = static_cast<ColorBoxesEngine*>(clientData)->world()->GetGravity();
    float length = gravity.Normalize();
    if (length == 0.0f) {
        gravity = b2Vec2(0, -1.0f);
    }
    gravity *= strength;
    static_cast<ColorBoxesEngine*>(clientData)->world()->SetGravity(gravity);
}

static void TW_CALL
getGravityDirection(void* value, void* clientData)
{
    b2Vec2 gravity = static_cast<ColorBoxesEngine*>(clientData)->world()->GetGravity();
    *static_cast<float*>(value) = atan2f(gravity.y, gravity.x) * 180.0f / M_PI;
}

static void TW_CALL
setGravityDirection(const void* value, void* clientData)
{
    float angle = *static_cast<const float*>(value) * M_PI / 180.0f;
    b2Vec2 gravity = static_cast<ColorBoxesEngine*>(clientData)->world()->GetGravity();
    float strength = gravity.Length();
    gravity = b2Vec2(cosf(angle), sinf(angle));
    gravity *= strength;
    static_cast<ColorBoxesEngine*>(clientData)->world()->SetGravity(gravity);
}

static void TW_CALL
resetWorldObjects(void* clientData)
{
    static_cast<ColorBoxesEngine*>(clientData)->resetWorld();
}


// Functional delete helpers
void deleteDoneShapes(Shape*& box)
{
    if (box->done()) {
        delete box;
        box = 0;
    }
}

template <class T>
void deleteAll(T*& object)
{
    delete object;
    object = 0;
}

template void deleteAll<Edge>(Edge*&);
template void deleteAll<Shape>(Shape*&);


// Static variables
const float ColorBoxesEngine::SELECTION_THRESHOLD = 10.0f;
const GLColor ColorBoxesEngine::SELECTION_COLOR = GLColor(1.0f, 0.0f, 0.0f, 1.0f);
const GLColor ColorBoxesEngine::EDGE_COLOR = GLColor::magenta().lighten(0.75f);
ColorBoxesEngine* ColorBoxesEngine::self = NULL;

ColorBoxesEngine*
ColorBoxesEngine::instance()
{
    return ColorBoxesEngine::self;
}

ColorBoxesEngine::ColorBoxesEngine(int w, int h, const char* resourcePath)
    : GameEngine(w, h, resourcePath),
      state_(NORMAL),
      currentShape_(TRIANGLE),
      newEdge_(NULL),
      selectedEdge_(NULL),
      renderStats_(false),
      gravity_(b2Vec2(0, -10.0f)),
      scaleFactor_(10.0f),
      yFlip_(-1.0f),
      textColor_(GLColor::white()),
      defaultCursor_(defaultCursor())
{
    // Set the cursor.
    SDL_SetCursor(defaultCursor_);
    
    // Set up Box2D world.
    bool doSleep = true;
    world_ = new b2World(gravity_);
    world_->SetAllowSleeping(doSleep);
    world_->SetWarmStarting(true);
    world_->SetContinuousPhysics(true);
    
    // Set up SDL TTF.
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf error: " << TTF_GetError() << std::endl;
    }
    
    std::string fontPath = std::string(resourcePath) + "/Arial.ttf";
    font_ = TTF_OpenFont(fontPath.c_str(), 12);
    if (font_ == NULL) {
        std::cerr << "SDL_ttf error: " << TTF_GetError() << std::endl;
    } else {
        TTF_SetFontStyle(font_, TTF_STYLE_BOLD);
    }
    
    // Set up AntTweakBar
    configureTweakBar();
    
    ColorBoxesEngine::self = this;
}

ColorBoxesEngine::~ColorBoxesEngine()
{
    delete world_;

    SDL_FreeCursor(defaultCursor_);
    TwTerminate();
    
    TTF_CloseFont(font_);
    TTF_Quit();
    SDL_Quit();
}

void
ColorBoxesEngine::configureTweakBar()
{
    TwInit(TW_OPENGL, NULL);
    TwWindowSize(width(), height());
    TwBar* tweakBar = TwNewBar("Color Boxes");
    TwDefine("GLOBAL help='Adjust Color Boxes parameters and perform commands.'");
    TwAddVarCB(tweakBar, "Objects", TW_TYPE_UINT32, NULL, getObjectCount, this, "");
    TwAddVarCB(tweakBar, "FPS", TW_TYPE_INT32, NULL, getFps, this, "");
    TwAddSeparator(tweakBar, "gravitySeparator", "");
    TwAddButton(tweakBar, "Gravity", NULL, NULL, "");
    TwAddVarCB(tweakBar, "Strength", TW_TYPE_FLOAT, setGravityStrength,
               getGravityStrength, this, "min=0 max=100 step=1.0");
    TwAddVarCB(tweakBar, "Direction", TW_TYPE_FLOAT, setGravityDirection,
               getGravityDirection, this, "min=-180 max=180 step=1.0");
    TwAddSeparator(tweakBar, "paramSeparator", "");
//    TwAddVarRW(tweakBar, "Scale", TW_TYPE_FLOAT, &scaleFactor_, "min=1.0 max=20.0 step=1.0");
    TwEnumVal modeEnumValues[] = {
        {NORMAL, "Normal"},
        {CREATE_OBJECT, "Create objects"},
        {CREATE_EDGE, "Create wall"},
        {DELETE_EDGE, "Delete wall"},
        {CREATE_BOMB, "Create bomb"}
    };
    TwType modeType = TwDefineEnum("ModeType", modeEnumValues, NUMBER_STATES);
    TwAddVarRW(tweakBar, "Mode", modeType, &state_, NULL);
    
    TwEnumVal shapeEnumValues[] = {
        {TRIANGLE, "Triangle"},
        {QUADRILATERAL, "Quadrilateral"},
        {PENTAGON, "Pentagon"},
        {HEXAGON, "Hexagon"},
        {HEPTAGON, "Heptagon"},
        {OCTAGON, "Octagon"},
        {CIRCLE, "Circle"},
        {BOX, "Box"}
    };
    TwType shapeType = TwDefineEnum("ShapeType", shapeEnumValues, NUMBER_OBJECT_SHAPES);
    TwAddVarRW(tweakBar, "Shape", shapeType, &currentShape_, NULL);
    TwAddButton(tweakBar, "Reset", resetWorldObjects, this, "");

}

void
ColorBoxesEngine::initializeData()
{
    setTitle("Box Fall");

    walls_.push_back(new Wall(0.0f, 1.0f,
                              5.0f, height() - 20.0f, this));
    walls_.push_back(new Wall(width() - 6.0f, 1.0f,
                              5.0f, height() - 20.0f, this));
    walls_.push_back(new Wall(width() / 16.0f, height() - 15.0f,
                              6.0f * width() / 16.0f, 5.0, this));
    walls_.push_back(new Wall(9.0f * width() / 16.0f, height() - 15.0f,
                              6.0f * width() / 16.0f, 5.0, this));
}

void
ColorBoxesEngine::update(long elapsedTime)
{
    const int32 velocityIterations = 8;
    const int32 positionIterations = 2;
    float32 timeStep = elapsedTime / 1000.0f;
    
    int x;
    int y;
    SDL_GetMouseState(&x, &y);

    // Create an object if we are in create mode.
    if (state_ == CREATE_OBJECT) {
        Shape* object = NULL;
        switch (currentShape_) {
            case TRIANGLE:
            case QUADRILATERAL:
            case PENTAGON:
            case HEXAGON:
            case HEPTAGON:
            case OCTAGON:
                object = new Polygon(x, y, currentShape_ + 3, this);
                break;
            case CIRCLE:
                object = new Circle(x, y, this);
                break;
            case BOX:
                object = new Box(x, y, this);
                break;
            default:
                // Do nothing.
                break;
        }
        
        objects_.push_back(object);
    } else if (state_ == DELETE_EDGE) {
        // Calculate the closest edge within a threshold radius.
        if (selectedEdge_) {
            selectedEdge_->setColor(EDGE_COLOR);
            selectedEdge_ = NULL;
        }
        b2Vec2 p(x, y);
        float minDistance = std::numeric_limits<float>::max();
        for (int i = 0; i < edges_.size(); ++i) {
            float distance = pointToEdgeDistance(p, edges_[i]);
            if (distance < SELECTION_THRESHOLD && distance < minDistance) {
                minDistance = distance;
                selectedEdge_ = edges_[i];
            }
        }
        if (selectedEdge_ != NULL) {
            selectedEdge_->setColor(SELECTION_COLOR);
        }
    }

    // Update the new edge if we are in edge drawing mode.
    if (newEdge_ != NULL) {
        newEdge_->setEndPoint(b2Vec2(x, y));
    }
    
    // Clear out any objects that are off the screen.
    for_each(objects_.begin(), objects_.end(), deleteDoneShapes);
    objects_.erase(std::remove(objects_.begin(), objects_.end(), static_cast<Shape*>(0)), objects_.end());
    
    world_->Step(timeStep, velocityIterations, positionIterations);
}

void
ColorBoxesEngine::render()
{
    TwDraw();
    for_each(walls_.begin(), walls_.end(), std::mem_fun(&Wall::render));
    for_each(objects_.begin(), objects_.end(), std::mem_fun(&Shape::render));
    for_each(edges_.begin(), edges_.end(), std::mem_fun(&Edge::render));

    if (newEdge_ != NULL) {
        newEdge_->render();
    }

    if (renderStats_) {
        renderStatistics();
    }
}

void
ColorBoxesEngine::renderStatistics()
{
    std::ostringstream msg;
    msg << objects_.size() << " objects, " << fps() << " fps";
//    renderText(msg.str(), 8.0f, 2.0f);
    ogl::drawString(8.0f, 16.0f, msg.str(), textColor_);
}

void
ColorBoxesEngine::renderText(const std::string &text, float x, float y)
{
    // Use SDL_TTF to render the text onto an initial surface.
    SDL_Surface* textSurface = TTF_RenderText_Blended(font_, text.c_str(), textColor_.toSDLColor());
        
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
        case SDLK_a:
	    currentShape_ = static_cast<ObjectShape>(static_cast<int>(currentShape_) + 1);
            if (currentShape_ == NUMBER_OBJECT_SHAPES) {
                currentShape_ = TRIANGLE;
            }
            std::cout << "Current shape: " << currentShape_ << std::endl;
            break;
        case SDLK_b:
            if (state_ == NORMAL) {
                state_ = CREATE_BOMB;
            }
            break;
        case SDLK_c:
            if (backgroundColor() == GLColor::black()) {
                setBackgroundColor(GLColor::white());
                textColor_ = GLColor::black();
            } else {
                setBackgroundColor(GLColor::black());
                textColor_ = GLColor::white();
            }
            break;
        case SDLK_d:
            if (state_ == NORMAL) {
                selectedEdge_ = NULL;
                state_ = DELETE_EDGE;
            }
            break;
        case SDLK_e:
            if (state_ == NORMAL) {
                state_ = CREATE_EDGE;
                selectedEdge_ = NULL;
            }
            break;
        case SDLK_r:
            // Reset the world: clear boxes and edges.
            resetWorld();
            break;
        case SDLK_s:
            renderStats_ = !renderStats_;
            break;
        case SDLK_SPACE:
            if (newEdge_ != NULL) {
                delete newEdge_;
                newEdge_ = NULL;
            }
            if (selectedEdge_ != NULL) {
                selectedEdge_->setColor(EDGE_COLOR);
                selectedEdge_ = NULL;
            }
            state_ = NORMAL;
            break;
        default:
            // Do nothing.
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
        switch (state_) {
            case NORMAL:
                state_ = CREATE_OBJECT;
                break;
            case CREATE_EDGE:
                if (newEdge_ == NULL) {
                    // Turn on draw edge mode and record the starting point.
                    b2Vec2 pt(x, y);
                    newEdge_ = new Edge(pt, pt, GLColor::cyan(), this);
                } else {
                    // End draw edge mode and finalize the edge.
                    b2Vec2 endPt(x, y);
                    newEdge_->setEndPoint(endPt);
                    newEdge_->setColor(EDGE_COLOR);
                    edges_.push_back(newEdge_);
                    newEdge_ = NULL;
                    state_ = NORMAL;
                }
                break;
            case DELETE_EDGE:
                delete selectedEdge_;
                edges_.erase(remove(edges_.begin(), edges_.end(), selectedEdge_), edges_.end());
                selectedEdge_ = NULL;
                state_ = NORMAL;
                break;
            case CREATE_BOMB:
                explode(b2Vec2(x, y));
                break;
            default:
                // Don't do anything.
                break;
        }
    }
}

void
ColorBoxesEngine::mouseButtonUp(int button, int x, int y, int dx, int dy)
{
    if (state_ == CREATE_OBJECT && button == SDL_BUTTON_LEFT) {
        state_ = NORMAL;
    }
}

unsigned long
ColorBoxesEngine::objectCount() const
{
    return objects_.size();
}

void
ColorBoxesEngine::explode(const b2Vec2& pos)
{
    b2Vec2 worldPos = coordPixelsToWorld(pos);
    
    // Create the query AABB.
    b2Vec2 diagonal(20, 20);
    b2Vec2 lowerBound = worldPos;
    lowerBound -= diagonal;
    b2Vec2 upperBound = worldPos;
    upperBound += diagonal;
    
    b2AABB bounds = {lowerBound, upperBound};

    // Set the query callback.
    ExplosionCallback explosion(worldPos);
    world()->QueryAABB(&explosion, bounds);
    
    state_ = NORMAL;
}

b2World*
ColorBoxesEngine::world()
{
    return world_;
}

void
ColorBoxesEngine::resetWorld()
{
    for_each(objects_.begin(), objects_.end(), deleteAll<Shape>);
    objects_.erase(std::remove(objects_.begin(), objects_.end(), static_cast<Shape*>(0)), objects_.end());
    
    for_each(edges_.begin(), edges_.end(), deleteAll<Edge>);
    edges_.erase(std::remove(edges_.begin(), edges_.end(), static_cast<Edge*>(0)), edges_.end());
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


