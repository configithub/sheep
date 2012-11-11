#include "Level.h"
#include "CApp.h"

Level Level::LevelInstance;

void Level::next(std::vector<CFollower*>& iSheeps, std::string& iLevelName, int iNbGrassPatches, int iPreLevelDuration, int iLevelDuration, int iNewSheepsIfSuccess) {
  _name = iLevelName;
  _duration = iLevelDuration;
  _preLevelDuration = iPreLevelDuration;
  _newSheepsIfSuccess = iNewSheepsIfSuccess;
  ++_levelNb;
  Rectangle aScreenRect(33, 33, WWIDTH-33, WHEIGHT-33);

  _grassPatches.clear();
  _grassPatches.reserve(iNbGrassPatches);
  for (int i = 0; i < iNbGrassPatches; i++) {
    GrassPatch aGrassPatch;
    _grassPatches.push_back(aGrassPatch);
    _grassPatches.back().generateRandom(aScreenRect, 40, 40, 80, 60);
    _grassPatches.back().setNbSheepsForValidation(1);
    _grassPatches.back().setNbSheepsValidated(0);
    _grassPatches.back().isValidated() = false;
  }


  int nbNewBombs = rand() %3 +1;

  for (int i = 0; i < nbNewBombs; i++) {
    int key = CEntity::CurrentEntityId;
    _bombs[key].generateRandom(aScreenRect);
  }

  if(_levelNb % 10 == 0 && !iSheeps.empty()) {
    int key = CEntity::CurrentEntityId;
    CApp::EntityPool[key].generateRandom(aScreenRect);
    iSheeps.push_back(&CApp::EntityPool[key]);  
  }
  if(iSheeps.empty()) {
    _gameOver = true;
  }

  _isRunning = false;
  _isOver = false;
  _levelSuccess = false;
  _startTime = SDL_GetTicks();

}

void Level::resetGrassPatches() {
  for(std::vector<GrassPatch>::iterator itGrassPatch = _grassPatches.begin();
      itGrassPatch != _grassPatches.end(); ++itGrassPatch) {
    itGrassPatch->reset();
  }
}

void Level::OnLoop(std::vector<CFollower*>& iSheeps) {
  int currentTime = SDL_GetTicks();
  if(currentTime - _startTime < _preLevelDuration) {
    // pre level : grass is not yet available
    _isRunning = false;
    return;
  }else{
    _isRunning = true;
  }
  if(_isRunning) {

    if( currentTime - _startTime < _duration ){
      // level is not over, check for validity condition


      for(std::map<int,Bomb>::iterator itBomb = _bombs.begin();
          itBomb != _bombs.end(); ++itBomb) {
        itBomb->second.explode(iSheeps);
      }


      _isRunning = true;
      for(std::vector<CFollower*>::iterator itSheep = iSheeps.begin();
          itSheep != iSheeps.end(); ++itSheep) {

        for(std::vector<GrassPatch>::iterator itGrassPatch = _grassPatches.begin();
            itGrassPatch != _grassPatches.end(); ++itGrassPatch) {

          if(itGrassPatch->isValidated()) { continue; }

          Rectangle intersection(0,0);
          (*itSheep)->updateCollisionMask();
          (*itSheep)->getCollisionMask().intersect(itGrassPatch->getPatch(), intersection);

          if(!intersection.isVoid()) {
            itGrassPatch->setNbSheepsValidated( itGrassPatch->getNbSheepsValidated() +1);
          }

          if( itGrassPatch->getNbSheepsValidated() >= itGrassPatch->getNbSheepsForValidation() ) {
            itGrassPatch->isValidated() = true;
          }
        }
      }
      _levelSuccess = true;
      if(_grassPatches.empty()) { _levelSuccess = false; } else{
        for(std::vector<GrassPatch>::iterator itGrassPatch = _grassPatches.begin();
            itGrassPatch != _grassPatches.end(); ++itGrassPatch) {
          if(itGrassPatch->isValidated() == false) { _levelSuccess = false; break; }
        }
      }
      // level is over, and player succeeded
      if(_levelSuccess) {
        resetGrassPatches();
        _isRunning = false;
        _isOver = true;
      } else { _isRunning = true; }

    } else {
      // level is over, and player failed
      resetGrassPatches();
      _levelSuccess = false;
      _isRunning = false;
      _isOver = true;

    }

  }


  std::vector<int> bombIdsToRemove;


  for(std::map<int,Bomb>::iterator itBomb = _bombs.begin();
      itBomb != _bombs.end(); ++itBomb) {
    if(itBomb->second.hasExploded()) {
      bombIdsToRemove.push_back(itBomb->second.getEntityId());
    }
  }

  for(std::vector<int>::iterator itId = bombIdsToRemove.begin(); 
      itId != bombIdsToRemove.end(); ++itId) { 
    //_bombs.erase(*itId);
  }  


}


void Level::OnRender() {

  resetTiles();

  if(_isRunning) {




    for(std::vector<GrassPatch>::iterator itGrassPatch = _grassPatches.begin();
        itGrassPatch != _grassPatches.end(); ++itGrassPatch) {
      // factor in collision offsets
      int StartX = itGrassPatch->getPatch().getCorner().getX() / TILE_SIZE;
      int StartY = itGrassPatch->getPatch().getCorner().getY() / TILE_SIZE;

      int EndX = (itGrassPatch->getPatch().getCorner().getX() + itGrassPatch->getPatch().getWidth() ) / TILE_SIZE;
      int EndY = (itGrassPatch->getPatch().getCorner().getY() + itGrassPatch->getPatch().getHeight() ) / TILE_SIZE;

      // determine which tiles our entity is over
      for(int iY = StartY; iY <= EndY; iY++) {
        for(int iX = StartX; iX <= EndX; iX++) {
          CTile* Tile = CArea::AreaControl.GetTile(iX * TILE_SIZE, iY * TILE_SIZE);
          if(Tile->TileID == 16 || Tile->TileID == 15) {
            Tile->TileID = 16; // grass
            Tile->TypeID = TILE_TYPE_GRASS; // grass
          }
        }
      }

    }

  }
}

void Level::resetTiles() {

  int StartX = 0;
  int StartY = 0;

  int EndX = WWIDTH / TILE_SIZE;
  int EndY = WHEIGHT / TILE_SIZE;

  // determine which tiles our entity is over
  for(int iY = StartY; iY < EndY; iY++) {
    for(int iX = StartX; iX < EndX; iX++) {
      CTile* Tile = CArea::AreaControl.GetTile(iX * TILE_SIZE, iY * TILE_SIZE);
      if(Tile == NULL) { continue; }
      if(Tile->TypeID == TILE_TYPE_GRASS) {
        Tile->TileID = 15; // grass
        Tile->TypeID = TILE_TYPE_NORMAL; // grass
      }
    }
  }


}

