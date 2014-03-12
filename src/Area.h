#ifndef _CAREA_H_
#define _CAREA_H_

#include "Map.h"

class Area {
  public:
    static Area            AreaControl;
    static Area            Background1;
    static Area            Background2;

  public:
    std::vector<Map>       MapList;

  private:
    int                     AreaSize;

    GLuint        glTileset;

  public:
    Area();

    bool    OnLoad(char* File);

    void    OnRender(int CameraX, int CameraY);

    void    OnCleanup();

    Map*    GetMap(int X, int Y);
    void     SetMap(int X, int Y, Map* imap);

    Tile*    GetTile(int X, int Y);
};

#endif
