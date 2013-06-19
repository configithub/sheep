#ifndef _CAREA_H_
    #define _CAREA_H_

#include "CMap.h"

class Area {
    public:
        static Area            AreaControl;

    public:
        std::vector<CMap>       MapList;

    private:
        int                     AreaSize;

        SDL_Surface*        Surf_Tileset;

    public:
        Area();

        bool    OnLoad(char* File);

        void    OnRender(SDL_Surface* Surf_Display, int CameraX, int CameraY);

        void    OnCleanup();

        CMap*    GetMap(int X, int Y);

        CTile*    GetTile(int X, int Y);
};

#endif
