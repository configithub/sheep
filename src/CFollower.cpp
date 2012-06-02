#include "CFollower.h"

#include <iostream>


CFollower::CFollower() {
  //std::cout << "creating sheep " << _entityId << std::endl;
    id = 0;
}

bool CFollower::OnLoad(char* File, int Width, int Height, int MaxFrames) {

  _sdlSurface = CSurface::Sprites[0];

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


  // give correct entity orientation
  if(_moveLeft) {
    _currentFrameCol = 0;
    if(_tileType == TILE_TYPE_GRASS) {
      _currentFrameCol+=2;
    }
  }else if(_moveRight) {
    _currentFrameCol = 1;
    if(_tileType == TILE_TYPE_GRASS) {
      _currentFrameCol+=2;
    }
  }

  _animControl.MinFrames = 1;
  if(_moveRight || _moveLeft || _moveUp || _moveDown) {
    _animControl.MaxFrames = 3;
  } else {
    _animControl.MaxFrames = 1;
  }

  if(_tileType == TILE_TYPE_GRASS) {
    if(!_moveRight && !_moveLeft && !_moveUp && !_moveDown) {
      _animControl.MinFrames = 0;
      _animControl.MaxFrames = 1;
    }
  }

  CEntity::OnAnimate();
}




