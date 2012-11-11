#ifndef EFFECT_H
#define EFFECT_H

#include "CEntity.h"

class Effect : public CEntity {

  public:

    Effect() : _id(0), _iterationNb(0), _maxIterationNb(1) {
      //_behavior = ENTITY_FLAG_GHOST;
      //std::cout << "creating effect " << _entityId << std::endl;
    }

    bool OnLoad(char* File, int Width, int Height, int MaxFrames);

    void OnRender(SDL_Surface* Surf_Display);

    void OnCleanup();
  
    void OnAnimate();

    const int& getId() const { return _id; }
    void setId(const int& id) { _id = id; }

    const int& getIterationNb() const { return _iterationNb; }
    void setIterationNb(const int& iterationNb) { _iterationNb = iterationNb; }

    const int& getMaxIterationNb() const { return _maxIterationNb; }
    void setMaxIterationNb(const int& maxIterationNb) { _maxIterationNb = maxIterationNb; }

  private:

    int _id;
    int _iterationNb;
    int _maxIterationNb;

};



#endif
