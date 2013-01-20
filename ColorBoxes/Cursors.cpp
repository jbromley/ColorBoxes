//
//  Cursors.cpp
//  ColorBo es
//
//  Created by Jay Bromley on 1/17/13.
//  Copyright (c) 2013 Jay Bromley. All rights reserved.
//

#include "Cursors.h"


static const char *defaultXpm[] = {
    /* width height num_colors chars_per_pixel */
    "    32    32        3            1",
    /* colors */
    "X c #ffffff",
    ". c #000000",
    "  c None",
    /* pixels */
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "              XXXX              ",
    "              X..X              ",
    "              X..X              ",
    "              X..X              ",
    "          XXXXX..XXXXX          ",
    "          X..........X          ",
    "          X..........X          ",
    "          XXXXX..XXXXX          ",
    "              X..X              ",
    "              X..X              ",
    "              X..X              ",
    "              XXXX              ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "                                ",
    "15,15"
};

SDL_Cursor* defaultCursor()
{
    return loadCursor(defaultXpm);
}

SDL_Cursor* loadCursor(const char *image[])
{
    Uint8 data[4 * 32];
    Uint8 mask[4 * 32];
    
    int i = -1;
    for (int row = 0; row < 32; ++row) {
        for (int col = 0; col < 32; ++col) {
            if (col % 8) {
                data[i] <<= 1;
                mask[i] <<= 1;
            } else {
                ++i;
                data[i] = mask[i] = 0;
            }
            switch (image[4 + row][col]) {
                case 'X':
                    mask[i] |= 0x01;
                    break;
                case '.':
                    data[i] |= 0x01;
                    mask[i] |= 0x01;
                    break;
                case ' ':
                    break;
            }
        }
    }

    int hotX, hotY;
    sscanf(image[36], "%d,%d", &hotX, &hotY);
    return SDL_CreateCursor(data, mask, 32, 32, hotX, hotY);
}
