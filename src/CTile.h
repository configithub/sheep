#ifndef _CTILE_H_
    #define _CTILE_H_

#include "Define.h"

enum {
    TILE_TYPE_NONE = 0,

    TILE_TYPE_NORMAL,
    TILE_TYPE_BLOCK,
    TILE_TYPE_GRASS,
    DOOR_STEP_RIGHT, //4 
    DOOR_STEP_LEFT, //5
    DOOR_STEP_UP, //6
    DOOR_STEP_DOWN, //7
    TRANSLATE_RIGHT, //8 
    TRANSLATE_LEFT, //9
    TRANSLATE_UP, //10
    TRANSLATE_DOWN, //11
    TILE_TYPE_HYPERTORIC
};

class CTile {
    public:
        int     TileID;
        int     TypeID;

    public:
        CTile();
};

#endif
