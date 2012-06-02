
#include "Bomb.h"
#include "CApp.h"
#include "Effect.h"

bool Bomb::OnLoad(char* File, int Width, int Height, int MaxFrames) {
  std::cout << "creating bomb " << _entityId << std::endl;

  _sdlSurface = CSurface::Sprites[1];
  
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

  _animControl.MinFrames = 0;
  _animControl.MaxFrames = 2;
  _animControl.Oscillate = true;

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
      int sheepEntityId = (*itSheep)->getEntityId();
      (*itSheep)->removeAtNextLoop() = true;
      itSheep = iSheeps.erase(itSheep);
    }else{
      ++itSheep;
    }

  }

  _hasExploded = true;

  // explosion effect

  int key = CEntity::CurrentEntityId;
  Effect& explosion = CApp::EffectPool[key];
  explosion.OnLoad("./gfx/explosion.png", 50, 32, 4); 
  explosion.setPosition(this->getPosition(), true);
  
}
