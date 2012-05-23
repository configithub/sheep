#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include "GrassPatch.h"

class Level {

  public:

    static Level LevelInstance;

    Level() : _isRunning(false){}

    Level(std::string& iLevelName, int& iNbGrassPatches, int& iLevelDuration, int& iNewSheepsIfSuccess) {
      this->next(iLevelName, iNbGrassPatches, iLevelDuration, iNewSheepsIfSuccess);
    }

    void next(std::string& iLevelName, int& iNbGrassPatches, int& iLevelDuration, int& iNewSheepsIfSuccess); 

    void OnLoop(std::vector<CFollower*>& iSheeps);

    void setStartTime(int iStartTime) { _startTime = iStartTime; }
    bool& isRunning() { return _isRunning; }

  private:

    std::string _name;
    std::vector<GrassPatch> _grassPatches;
    int _duration;
    bool _levelSuccess;
    int _newSheepsIfSuccess;
    int _startTime;
    bool _isRunning;

};


#endif
