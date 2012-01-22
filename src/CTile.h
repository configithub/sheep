#ifndef _CTILE_H_
    #define _CTILE_H_

#include "Define.h"

enum {
    TILE_TYPE_NONE = 0,

    TILE_TYPE_NORMAL,
    TILE_TYPE_BLOCK,
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
