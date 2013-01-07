#include <SDL/SDL.h>
#include <sys/param.h> /* for MAXPATHLEN */
#include <unistd.h>
#include "ColorBoxesEngine.h"


int main(int argc, char* argv[])
{
    const char *resourcePath = "./ColorBoxes";
    ColorBoxesEngine* game = new ColorBoxesEngine(1680, 1050, resourcePath);
    game->init();
    int status = game->run();

    /* We're done, thank you for playing */
    delete game;
    return status;
}
