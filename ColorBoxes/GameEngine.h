#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string> 
#include <SDL/SDL.h>
 
/** The base engine class. */
class GameEngine  
{
public:
    GameEngine(int width, int height, const std::string& resourcePath = "");
    virtual ~GameEngine();
    
    int init();
    int run();
    
    /**
     * Initializes all game-specific data at game startup.
     */
    virtual void initializeData	() {}
    
    /**
     * Does all game calculation and updating.
     * @param elapsedTime The time in milliseconds elapsed since the
     * function was called last.
     */
    virtual void update(long elapsedTime) {}
    
    /**
     * Does all game rendering.
     */
    virtual void render() {}
    
    /**
     * Cleans up all allocated data and resources.
     */
    virtual void end() {}
    
    /**
     * Perform actions when the window becomes active again.
     */
    virtual void windowActive() {}
    
    /**
     * Perform actions when the window becomes inactive.
     */
    virtual void windowInactive() {}
    
    /**
     * Handles when a keyboard key has been released.
     * @param keyNum The key number.
     */
    virtual void keyUp(int keyNum) {}
    
    /**
     * Handles when a keyboard key has been pressed.
     * @param keyNum The key number.
     */
    virtual void keyDown(int keyNum) {}
    
    /**
     * Handles when the mouse has been moved.
     * @param button Specifies if a mouse button is pressed.
     * @param x	The mouse position on the X-axis in pixels.
     * @param y	The mouse position on the Y-axis in pixels.
     * @param dx The mouse position on the X-axis relative to the last
     *           position, in pixels.
     * @param dy The mouse position on the Y-axis relative to the last
     *           position, in pixels.
     * @bug The button variable is always NULL.
     */
    virtual void mouseMoved(int button, int x, int y, int dx, int dy) {}
    
    /**
     * Handles when a mouse button has been released.
     * @param button Specifies if a mouse button is pressed.
     * @param x The mouse position on the X-axis in pixels.
     * @param y	The mouse position on the Y-axis in pixels.
     * @param dx The mouse position on the X-axis relative to the last
     *           position, in pixels.
     * @param dy The mouse position on the Y-axis relative to the last
     *           position, in pixels.
     */
    virtual void mouseButtonUp(int button, int x, int y, int dx, int dy) {}
    
    /**
     * Handles when a mouse button has been pressed.
     * @param button Specifies if a mouse button is pressed.
     * @param x The mouse position on the X-axis in pixels.
     * @param y	The mouse position on the Y-axis in pixels.
     * @param dx The mouse position on the X-axis relative to the last
     *           position, in pixels.
     * @param dy The mouse position on the Y-axis relative to the last
     *           position, in pixels.
     */
    virtual void mouseButtonDown(int button, int x, int y, int dx, int dy) {}
    
    void setTitle(const std::string& title);
    std::string title() const;
    SDL_Surface* surface();
    int height() const;
    int width() const;
    void setBackgroundColor(Uint32 backgroundColor);
    Uint32 backgroundColor() const;
    int fps() const;
    std::string resourcePath() const;
    void setResourcePath(const std::string& resourcePath);

protected:
    void doUpdate();
    void doRender();
    void setSize(int iWidth, int iHeight);
    virtual void handleInput();

    /** Last iteration's tick value */
    long lastTick_;
    
    /** Has quit been called? */
    bool quit_;
   
    /** Is the window minimized? */
    bool minimized_;
    
private:

    /** Window width */
    int width_;

    /** Window height */
    int height_;
    
    /** Background color */
    Uint32 backgroundColor_;
    
    /** The title of the window */
    std::string title_;
 
    /** Screen surface */
    SDL_Surface* screen_;
 
    /* Variables to calculate the frame rate */
    /** Tick counter. */
    int fpsTickCounter_;
 
    /** Frame rate counter. */
    int fpsCounter_;
 
    /** Stores the last calculated frame rate. */
    int currentFps_;
    
    /** Stores the resource path. */
    std::string resourcePath_;
 };
 
#endif // GAMEENGINE_H
