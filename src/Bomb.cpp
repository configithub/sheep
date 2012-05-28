
#include "Bomb.h"
#include "CApp.h"

bool Bomb::OnLoad(char* File, int Width, int Height, int MaxFrames) {
  Rectangle mask(0,0,Width,Height);
  if(CEntity::OnLoad(File, mask, MaxFrames) == false) {
    return false;
  }

  return true;
}


void Bomb::OnRender(SDL_Surface* Surf_Display) {
  //if(!_hasExploded) {
    CEntity::OnRender(Surf_Display);
  //}
}

void Bomb::OnCleanup() {
  CEntity::OnCleanup();
}


void Bomb::OnAnimate() {


  // give correct entity orientation
  if(_moveLeft) {
    _currentFrameCol = 0;
  }else if(_moveRight) {
    _currentFrameCol = 1;
  }

  _animControl.MinFrames = 1;
  _animControl.MinFrames = 1;
  //_animControl.Oscillate = true;

  CEntity::OnAnimate();

}

void Bomb::generateRandom(Rectangle& boundaries) {
  int x,y;
  x = rand() % (boundaries.getWidth() ) + 1;
  y = rand() % (boundaries.getHeight() ) + 1;

  _position.set(x,y);
  _nextPosition.set(x,y);

  OnLoad("./gfx/bomb.png", 32, 32, 4);

  CEntity::EntityList.push_back(this);
}



void Bomb::explode(std::vector<CFollower*>& iSheeps) {


  int currentTime = SDL_GetTicks();
  if(currentTime - _startTime < _delay) {
    // bomb did not explode yet
    return;
  }

  // slightly dirty loop
  if(_hasExploded) { return; }
  for(std::vector<CFollower*>::iterator itSheep = iSheeps.begin();
      itSheep != iSheeps.end(); ) {

    PointDouble dist;
    CEntity::dist( *this, **itSheep, dist);
    if(dist.modulus() < _radius ) {
      // sheep died in explosion
      (*itSheep)->OnCleanup();
      int sheepEntityId = (*itSheep)->getEntityId();
      //delete *itSheep;
      itSheep = iSheeps.erase(itSheep);
      CApp::EntityPool.erase(sheepEntityId);
    }else{
      ++itSheep;
    }

  }

  _hasExploded = true;
  
  //delete this;

}
