#include "Level.h"


Level Level::LevelInstance;

void Level::next(std::string& iLevelName, int& iNbGrassPatches, int& iLevelDuration, int& iNewSheepsIfSuccess) {
  _name = iLevelName;
  _duration = iLevelDuration;
  _newSheepsIfSuccess = iNewSheepsIfSuccess;

  Rectangle aScreenRect(0, 0, WWIDTH, WHEIGHT);

  for (i = 0; i < iNbGrassPatches; i++) {
    GrassPatch aGrassPatch;
    _grassPatches.push_back(aGrassPatch);
    _grassPatches.back().generateRandom(aScreenRect, 40, 40, 80, 60);
    _grassPatches.back().setNbSheepsForValidation(1);
  }

  _isRunning = true;
  _startTime = SDL_GetTicks();
}

void Level::OnLoop(std::vector<CFollower*>& iSheeps) {

  if(_isRunning) {

    int currentTime = SDL_GetTicks();
    if( currentTime - _startTime < _duration ){
      // level is not over, check for validity condition
      for(std::vector<CFollower*>::iterator itSheep = iSheeps.begin(); 
          itSheep != iSheeps.end(); ++itSheep) { 

        int sheepTileId = CArea::AreaControl.GetTile(itSheep->getMask().getCenter().getX(), itSheep->getMask().getCenter().getY() ).TileID;
        for(std::vector<GrassPatch>::iterator itGrassPatch = _grassPatches.begin(); 
            itGrassPatch != _grassPatches.end(); ++itGrassPatch) { 

          if(itGrassPatch->isValidated()) { continue; }

          for(std::vector<int>::iterator itTileId = itGrassPatch->getTiles().begin(); 
              itTileId != itGrassPatch->getTiles().end(); ++itTileId) { 
            if(*itTileId == sheepTileId) {
              itGrassPatch->getNbSheepsValidated()++;
              break;
            }
          }  
          if( itGrassPatch->getNbSheepsValidated() == itGrassPatch->getNbSheepsForValidation() ) {
            itGrassPatch->isValidated() = true;
          }

        }  

      }  
      _levelSuccess = true;
      for(std::vector<GrassPatch>::iterator itGrassPatch = _grassPatches.begin(); 
          itGrassPatch != _grassPatches.end(); ++itGrassPatch) { 
        if(itGrassPatch->isValidated() == false) { _levelSuccess = false; break; } 
      }  

      if(_levelSuccess) { _isRunning = false; } else { _isRunning = true; }

    } else {
      // level is over, and player failed
      _levelSuccess = false;
      _isRunning = false;  

    }

  }

}
