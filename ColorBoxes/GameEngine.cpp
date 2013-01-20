#include "GameEngine.h"
#include <iostream>
#include <AntTweakBar.h>


GameEngine::GameEngine(int width, int height, const std::string& resourcePath)
    : lastTick_(0),
    quit_(false),
    minimized_(false),
    width_(width),
    height_(height),
    backgroundColor_(GLColor::black()),
    screen_(0),
    fpsTickCounter_(0),
    fpsCounter_(0),
    currentFps_(0),
    resourcePath_(resourcePath)
{
    // Register SDL_Quit to be called at exit; makes sure things are
    // cleaned up when we quit.
    atexit(SDL_Quit);
    
    // Initialize SDL's subsystems - in this case, only video.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Unable to init SDL: " << SDL_GetError() << "."
        << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Set up OpenGL.
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    // The only Linux box I've tested has an old video card, so it doesn't
    // support all of these attributes. I need to figure out the best 
    // attributes supported by any given machine.
#ifdef __APPLE__    
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
#endif

    // Attempt to create a window with the specified height and width.
    setSize(width_, height_);
    if (screen_ == NULL) {
        std::cerr << "Unable to set up video: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    GLColor& c = backgroundColor_;
    glClearColor(c.r, c.g, c.b, c.a);
    glClearDepth(1.0f);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, 1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
}

/** Destructor. **/
GameEngine::~GameEngine()
{
    SDL_Quit();
}

void GameEngine::setSize(int width, int height)
{
    width_  = width;
    height_ = height;
    screen_ = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL);
//    screen_ = SDL_SetVideoMode(width, height, 0, SDL_SWSURFACE);
}

int GameEngine::init()
{
    initializeData();
    
    return EXIT_SUCCESS;
}

int GameEngine::run()
{
    lastTick_ = SDL_GetTicks();
    quit_ = false;
    
    // Main loop: loop forever.
    while (!quit_) {
        // Handle mouse and keyboard input
        handleInput();
        
        if (!minimized_) {
                // Do some thinking
            doUpdate();
            
            // Render stuff
            doRender();
        }
    }
    
    end();
    
    return EXIT_SUCCESS;
}

void GameEngine::handleInput()
{
    // Poll for events, and handle the ones we care about.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        int handled = TwEventSDL(&event, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
        if (!handled) {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    // If escape is pressed set the Quit-flag
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        quit_ = true;
                        break;
                    }
                    
                    keyDown(event.key.keysym.sym);
                    break;
                    
                case SDL_KEYUP:
                    keyUp(event.key.keysym.sym);
                    break;
                    
                case SDL_QUIT:
                    quit_ = true;
                    break;
                    
                case SDL_MOUSEMOTION:
                    mouseMoved(event.button.button,
                               event.motion.x, event.motion.y,
                               event.motion.xrel, event.motion.yrel);
                    break;
                    
                case SDL_MOUSEBUTTONUP:
                    mouseButtonUp(event.button.button,
                                  event.motion.x, event.motion.y,
                                  event.motion.xrel, event.motion.yrel);
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    mouseButtonDown(event.button.button,
                                    event.motion.x, event.motion.y,
                                    event.motion.xrel, event.motion.yrel);
                    break;
                    
                case SDL_ACTIVEEVENT:
                    if (event.active.state & SDL_APPACTIVE) {
                        if (event.active.gain) {
                            minimized_ = false;
                            windowActive();
                        } else {
                            minimized_ = true;
                            windowInactive();
                        }
                    }
                    break;
            }
        }
    }
}

void GameEngine::doUpdate()
{
    long elapsedTicks = SDL_GetTicks() - lastTick_;
    lastTick_ = SDL_GetTicks();
    
    update(elapsedTicks);
    
    fpsTickCounter_ += elapsedTicks;
}

void GameEngine::doRender()
{
    ++fpsCounter_;
    if (fpsTickCounter_ >= 1000) {
        currentFps_ = fpsCounter_;
        fpsCounter_ = 0;
        fpsTickCounter_ = 0;
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    render();
    SDL_GL_SwapBuffers();
}

void GameEngine::setTitle(const std::string& title)
{
    title_ = title;
    SDL_WM_SetCaption(title.c_str(), 0);
}

std::string GameEngine::title() const
{
    return title_;
}

SDL_Surface* GameEngine::surface()
{ 
    return screen_;
}

int
GameEngine::width() const
{
    return width_;
}

int
GameEngine::height() const
{
    return height_;
}

void
GameEngine::setBackgroundColor(const GLColor& backgroundColor)
{
    backgroundColor_ = backgroundColor;
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
}

GLColor
GameEngine::backgroundColor() const
{
    return backgroundColor_;
}

int
GameEngine::fps() const
{
    return currentFps_;
}

std::string
GameEngine::resourcePath() const
{
    return resourcePath_;
}

void
GameEngine::setResourcePath(const std::string& resourcePath)
{
    resourcePath_ = resourcePath;
}

