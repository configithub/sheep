#ifndef BOMB_H
#define BOMB_H

#include "CFollower.h"

class Bomb : public CEntity {

  public:

    Bomb(): _delay(5000), _radius(80), _hasExploded(false) {
      _startTime = SDL_GetTicks();

    }

    Bomb(int iDelay, int iRadius) : _delay(iDelay), _radius(iRadius), _hasExploded(false) {
      _startTime = SDL_GetTicks();

    }

    void generateRandom(Rectangle& boundaries);

    void explode(std::vector<CFollower*>& iSheeps);

    bool OnLoad(char* File, int Width, int Height, int MaxFrames);

    bool hasExploded() { return _hasExploded; }

    void OnRender(SDL_Surface* Surf_Display);

    void OnCleanup();


    void OnAnimate();

  private:

    int _startTime;
    int _delay;
    int _radius;
    bool _hasExploded;

};



#endif
