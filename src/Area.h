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

        GLuint        glTileset;

    public:
        Area();

        bool    OnLoad(char* File);

        void    OnRender(int CameraX, int CameraY);

        void    OnCleanup();

        CMap*    GetMap(int X, int Y);
        void     SetMap(int X, int Y, CMap* imap);

        CTile*    GetTile(int X, int Y);
};

#endif
