#include "Map.h"
#include "Camera.h"
#include <iostream>

Map::Map() {
  glTileset = 0;
  Map_X = 0;
  Map_Y = 0;
}


bool Map::OnLoad(char* File) {
  TileList.clear();
  FILE* FileHandle = fopen(File, "r");
  if(FileHandle == NULL) { return false; }
  for(int Y = 0;Y < MAP_HEIGHT;Y++) {
    for(int X = 0;X < MAP_WIDTH;X++) {
      Tile tempTile;
      fscanf(FileHandle, "%d:%d ", &tempTile.TileID,
                                 &tempTile.TypeID);
      TileList.push_back(tempTile);
    }
    fscanf(FileHandle, "\n");
  }
  fclose(FileHandle);
  return true;
}


void Map::OnRender(int MapX, int MapY) {
  if(glTileset == 0) { return; }
  int& w = Texture::WHmap[glTileset].first;
  int& h = Texture::WHmap[glTileset].second;
  int TilesetWidth  = w / TILE_SIZE;
  int TilesetHeight = h / TILE_SIZE;
  // TODO to avoid doing this : the tileset picture must be a power of 2 square...
  TilesetWidth = 6;
  TilesetHeight = 7;

  int ID = 0;
  for(int Y = 0;Y < MAP_HEIGHT;Y++) {
    for(int X = 0;X < MAP_WIDTH;X++) {
      if(TileList[ID].TypeID == TILE_TYPE_NONE) {
        ID++;
        continue;
      }
      int tX = MapX + (X * TILE_SIZE);
      int tY = MapY + (Y * TILE_SIZE);

      int TilesetX = (TileList[ID].TileID % TilesetWidth) * TILE_SIZE;
      int TilesetY = (TileList[ID].TileID / TilesetWidth) * TILE_SIZE;

      // dont draw tiles which are not currently visible
      //if( tX >= -TILE_SIZE && tX <= WWIDTH
      //&& tY >= -TILE_SIZE && tY <= WHEIGHT ) {
      if( tX >= -WWIDTH/2 - TILE_SIZE && tX <= WWIDTH/2 
          && tY >= -WHEIGHT/2 - TILE_SIZE && tY <= WHEIGHT/2 ) {
        Texture::draw(glTileset, tX, tY, TilesetX, TilesetY, TILE_SIZE, TILE_SIZE);
      }
      ID++;
    }
    }
  }


  Tile* Map::GetTile(int X, int Y) {
    int ID = 0;

    ID = X / TILE_SIZE;
    ID = ID + (MAP_WIDTH * (Y / TILE_SIZE));

    if(ID < 0 || ID >= TileList.size()) {
      return NULL;
    }

    return &TileList[ID];
  }
