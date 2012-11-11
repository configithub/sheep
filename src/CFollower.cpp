#include "CFollower.h"
#include "CApp.h"

#include <iostream>


CFollower::CFollower() : _selection(0){ }

void CFollower::generateRandom(Rectangle& boundaries) {
  std::cout<< "CFollower generateRandom" << std::endl;
  
  int x,y;
  x = abs (std::min( (int)(rand() % (boundaries.getWidth() ) + boundaries.getCorner().getX()), WWIDTH-33) );
  y = abs (std::min( (int)(rand() % (boundaries.getHeight() ) + boundaries.getCorner().getY()), WHEIGHT-33) );

  _position.set(x,y);
  _nextPosition.set(x,y);

  _sdlSurface = CSurface::Sprites[0];

  Rectangle mask(0,0,32,32);
  OnLoad("./gfx/sheep6.png", mask, 4);

  if(!this->PosValidOnMap(_position)) { this->generateRandom(boundaries); }else{
    CEntity::EntityList.push_back(this);
  }

}


void CFollower::OnLoop() {
  if(!_removeAtNextLoop) {
    CEntity::NextEntityList.push_back(this);
  }else{
    CEntity::EntityListToRemove.push_back(this);
  }
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

void CFollower::OnRender(SDL_Surface* Surf_Display) {
  CEntity::OnRender(Surf_Display);
}

void CFollower::OnCleanup() {
  //CEntity::OnCleanup();
  CApp::EntityPool.erase(_entityId);
}



