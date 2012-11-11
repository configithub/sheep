#ifndef _CFollower_H_
#define _CFollower_H_

#include "CEntity.h"

class CFollower : public CEntity {
  public:
    CFollower();

    void generateRandom(Rectangle& boundaries);

    void OnLoop();

    void OnAnimate();

    void OnRender(SDL_Surface* Surf_Display);

    void OnCleanup();

    int _selection;

};

#endif
