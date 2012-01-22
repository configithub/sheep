#ifndef _CFollower_H_
#define _CFollower_H_

#include "CEntity.h"

class CFollower : public CEntity {
  public:
    CFollower();

    bool OnLoad(char* File, int Width, int Height, int MaxFrames);

    void OnLoop();

    bool Follow(CEntity* Entity);

    void OnRender(SDL_Surface* Surf_Display);

    void OnCleanup();

    void OnAnimate();

    bool OnCollision(CEntity* Entity, CEntityCol* EntityCol, int nbCollisionCheck, int maxCollisionCheck) ;

  public:

    CEntity* FollowedEntity;

    int id;
};

#endif
