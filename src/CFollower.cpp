#include "CFollower.h"

#include <iostream>

CFollower::CFollower() {
    FollowedEntity = NULL;
    id = 0;
}

bool CFollower::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    if(CEntity::OnLoad(File, Width, Height, MaxFrames) == false) {
        return false;
    }

    return true;
}

void CFollower::OnLoop() {
    CEntity::OnLoop();
    if(CanFollow) {
        Follow(FollowedEntity);
    }
}


void CFollower::OnRender(SDL_Surface* Surf_Display) {
    CEntity::OnRender(Surf_Display);
}

void CFollower::OnCleanup() {
    CEntity::OnCleanup();
}


void CFollower::OnAnimate() {
    if(MoveRight || MoveLeft) {
        Anim_Control.MaxFrames = 4;
    } else {
        Anim_Control.MaxFrames = 0;
    }

    CEntity::OnAnimate();
}

// follow another entity
bool CFollower::Follow(CEntity* Entity) {
    if(CanFollow) {
        if( X - Entity->X > 0) {
            if( X - Entity->X > 80) {
                MoveLeft = true; /*if(!PosValidOnMap(X+5,Y)) { Jump(); }*/
            }
            if( X - Entity->X <= 80 && X - Entity->X >= 40) {
                MoveLeft = false;
                MoveRight = false;
            }
            if( X - Entity->X < 40) {
                MoveRight = true;
            }
        } else {
            if(Entity->X - X > 80) {
                MoveRight = true;
            }
            if(Entity->X - X <= 80 && Entity->X - X >= 40) {
                MoveRight = false;
                MoveLeft = false;
            }
            if(Entity->X - X < 40) {
                MoveLeft = true; /*if(!PosValidOnMap(X-5,Y)) { Jump(); }*/
            }
        }

        if( Y - Entity->Y > 60 ) {
            Jump();
        }
    }
}


bool CFollower::OnCollision(CEntity* Entity, CEntityCol* EntityCol, int nbCollisionCheck, int maxCollisionCheck) {

    //std::cout << "Entity " << id << " offsetX: " << OffsetX << std::endl;
    //std::cout << "Entity " << Entity->id << " offsetX: " << Entity->OffsetX << std::endl;
    std::cout << "Collision: " << Entity->entityId << "(" << Entity->SpeedX << ")" << "->" << entityId << "(" << SpeedX << ")" << " : (" << EntityCol->Height << ", " << EntityCol->Width << ")" << std::endl;
   // std::cout << "Collision: " << Entity->entityId << "(" << Entity->X << ", " <<  Entity->Y << ")" << "->" << entityId << "(" << X << ", " <<  Y << ")" << " : (" << EntityCol->Height << ", " << EntityCol->Width << ")" << std::endl;



}



