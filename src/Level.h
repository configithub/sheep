#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include "GrassPatch.h"
#include "CFollower.h"
#include "Bomb.h"

class Level {

  public:

    static Level LevelInstance;

    Level() : _isRunning(false), _levelSuccess(false), _gameOver(false){}

    Level(std::vector<CFollower*>& iSheeps, std::string& iLevelName, int& iNbGrassPatches, int& iPreLevelDuration, int& iLevelDuration, int& iNewSheepsIfSuccess) : _isRunning(false), _levelSuccess(false) {
      this->next(iSheeps, iLevelName, iNbGrassPatches, iPreLevelDuration, iLevelDuration, iNewSheepsIfSuccess);
    }

    void next(std::vector<CFollower*>& iSheeps, std::string& iLevelName, int iNbGrassPatches, int iPreLevelDuration, int iLevelDuration, int iNewSheepsIfSuccess);

    void OnLoop(std::vector<CFollower*>& iSheeps);

    void setStartTime(int iStartTime) { _startTime = iStartTime; }
    bool& isRunning() { return _isRunning; }
    bool& isOver() { return _isOver; }
    bool& isLevelSuccess() { return _levelSuccess; }

    void OnRender();
    void resetTiles();
    void resetGrassPatches();

    std::map<int, Bomb>& getBombs() { return _bombs; } 
    
    bool isGameOver() { return _gameOver; }

  private:

    std::string _name;
    std::vector<GrassPatch> _grassPatches;
    std::map<int, Bomb> _bombs;
    int _preLevelDuration;
    int _duration;
    bool _levelSuccess;
    int _newSheepsIfSuccess;
    int _startTime;
    bool _isRunning;
    bool _isOver;
    int _levelNb;
    bool _gameOver;
};


#endif
