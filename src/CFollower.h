#ifndef _CFollower_H_
#define _CFollower_H_

#include "CEntity.h"

class CFollower : public CEntity {
  public:
    CFollower();

    bool OnLoad(char* File, int Width, int Height, int MaxFrames);

    void OnRender(SDL_Surface* Surf_Display);

    void OnCleanup();

    void OnAnimate();

    int id;

};

#endif
