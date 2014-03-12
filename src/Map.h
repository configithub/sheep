#ifndef _CMAP_H_
#define _CMAP_H_

#include <SDL.h>
#include <vector>

#include "Tile.h"
#include "Texture.h"

// A map is a grid of tile the same size of the window
class Map {
  public:
    GLuint glTileset;

  private:
    std::vector<Tile> TileList;

  public:

    float Map_X;
    float Map_Y;

    Map();

  public:
    // load a map from file
    bool OnLoad(char* File);

    void OnRender(int MapX, int MapY);

    Tile* GetTile(int X, int Y);
};

#endif
