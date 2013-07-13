#include "Area.h"
#include "Define.h"
#include <iostream>
Area Area::AreaControl;

Area::Area() {
    AreaSize = 0;
}

// load the area file containing all our map definitions
bool Area::OnLoad(char* File) {
    MapList.clear();

    FILE* FileHandle = fopen(File, "r");

    if(FileHandle == NULL) {

        return false;
    }

    char TilesetFile[255];

    fscanf(FileHandle, "%s\n", TilesetFile);

    if(!CSurface::load(TilesetFile)) {
        fclose(FileHandle);
        std::cout << "file handle false: " << TilesetFile;
        return false;
    }
    glTileset = CSurface::Sprites.back();

    fscanf(FileHandle, "%d\n", &AreaSize);

    for(int X = 0;X < AreaSize;X++) {
        for(int Y = 0;Y < AreaSize;Y++) {
            char MapFile[255];

            fscanf(FileHandle, "%s ", MapFile);

            CMap tempMap;
            tempMap.Map_X = X * MAP_WIDTH * TILE_SIZE;
            tempMap.Map_Y = Y * MAP_HEIGHT * TILE_SIZE;
            if(tempMap.OnLoad(MapFile) == false) {
                fclose(FileHandle);

                return false;
            }

            tempMap.glTileset = glTileset;

            MapList.push_back(tempMap);
        }
        fscanf(FileHandle, "\n");
    }

    fclose(FileHandle);

    return true;
}

// renders the four maps visible on screen (overlapping with it)
void Area::OnRender(int CameraX, int CameraY) {
    int MapWidth  = MAP_WIDTH * TILE_SIZE;
    int MapHeight = MAP_HEIGHT * TILE_SIZE;

    // calculate the id of the map were the camera resides now
    int FirstID = -CameraX / MapWidth;
        FirstID = FirstID + ((-CameraY / MapHeight) * AreaSize);

    // calculate the 4 ids of the 4 maps overlaping with the screen view
    for(int i = 0;i < 4;i++) {
        int ID = FirstID + ((i / 2) * AreaSize) + (i % 2);

        if(ID < 0 || ID >= MapList.size()) continue;

        int X = ((ID % AreaSize) * MapWidth) + CameraX;
        int Y = ((ID / AreaSize) * MapHeight) + CameraY;

        // renders the four maps
        MapList[ID].OnRender(X, Y);
    }
}

// get map at coordinate
CMap* Area::GetMap(int X, int Y) {
    int MapWidth  = MAP_WIDTH * TILE_SIZE;
    int MapHeight = MAP_HEIGHT * TILE_SIZE;

    int ID = X / MapWidth;
        ID = ID + ((Y / MapHeight) * AreaSize);

    if(ID < 0 || ID >= MapList.size()) {
        return NULL;
    }

    return &MapList[ID];
}

// get tile at coordinate
CTile* Area::GetTile(int X, int Y) {
    int MapWidth  = MAP_WIDTH * TILE_SIZE;
    int MapHeight = MAP_HEIGHT * TILE_SIZE;

    CMap* Map = GetMap(X, Y);

    if(Map == NULL) return NULL;

    X = X % MapWidth;
    Y = Y % MapHeight;

    return Map->GetTile(X, Y);
}


// cleans the entire area
void Area::OnCleanup() {
    if(glTileset) {
       // SDL_FreeSurface(glTileset);
    }

    MapList.clear();
}
