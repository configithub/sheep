#include "CFollower.h"

#include <iostream>

CFollower::CFollower() {
    id = 0;
}

bool CFollower::OnLoad(char* File, int Width, int Height, int MaxFrames) {
    Rectangle mask(0,0,Width,Height);
    if(CEntity::OnLoad(File, mask, MaxFrames) == false) {
        return false;
    }

    return true;
}


void CFollower::OnRender(SDL_Surface* Surf_Display) {
    CEntity::OnRender(Surf_Display);
}

void CFollower::OnCleanup() {
    CEntity::OnCleanup();
}


void CFollower::OnAnimate() {
    if(_moveRight || _moveLeft || _moveUp || _moveDown) {
        _animControl.MaxFrames = 4;
    } else {
        _animControl.MaxFrames = 0;
    }

    CEntity::OnAnimate();
}




