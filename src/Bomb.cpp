
#include "Bomb.h"
#include "CApp.h"
#include "Effect.h"
#include "Level.h" 

bool Bomb::OnLoad(char* File, int Width, int Height, int MaxFrames) {
  //std::cout << "creating bomb " << _entityId << std::endl;

  _sdlSurface = CSurface::Sprites[1];
  
  Rectangle mask(0,0,Width,Height);
  if(CEntity::OnLoad(File, mask, MaxFrames) == false) {
    return false;
  }

  return true;
}

void Bomb::OnRender(SDL_Surface* Surf_Display) {
  int remaining = std::max(0, (int) (_delay - (SDL_GetTicks() - _startTime)) /1000);
  CEntity::OnRender(Surf_Display);
  CApp::print_num(Surf_Display, CApp::Surf_NumFontBomb, _position.getX(), _position.getY(), remaining ); 
}

void Bomb::OnLoop() {
  explode(CApp::Sheeps);  
  if(!_removeAtNextLoop) {
    CEntity::NextEntityList.push_back(this);
  }else{
    CEntity::EntityListToRemove.push_back(this);
  }
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
  x = abs (std::min( (int)(rand() % (boundaries.getWidth() ) + boundaries.getCorner().getX()), boundaries.getCorner().getX() ) );
  y = abs (std::min( (int)(rand() % (boundaries.getHeight() ) + boundaries.getCorner().getY()), boundaries.getCorner().getY() ) );

  _position.set(x,y);
  _nextPosition.set(x,y);

  OnLoad("./gfx/bomb.png", 32, 32, 4);
  //std::cout << "bomb delay: " << _delay << std::endl;

  CEntity::EntityList.push_back(this);
}

void Bomb::OnCleanup() {
  //CEntity::OnCleanup();
  CApp::BombPool.erase(_entityId); 
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
  for(std::map<int,Bomb>::iterator itBomb = Level::LevelInstance.getBombs().begin(); 
    itBomb != Level::LevelInstance.getBombs().end(); ++itBomb) { 
    if(itBomb->second.getEntityId() != _entityId) {
      PointDouble dist;
      CEntity::dist( *this, itBomb->second, dist);
      if(dist.modulus() < _radius ) {
        // chain explosion
        itBomb->second._delay = 1500;
      }
    }
  } 


  _hasExploded = true;
  _removeAtNextLoop = true;

  /*
  // explosion effect
  int nbExplosionEffect = rand() % 5+3;
  for (int i = 0; i < nbExplosionEffect; i++) {
    int key = CEntity::CurrentEntityId;
    Effect& explosion = CApp::EffectPool[key];
    CApp::EffectPool[key].setParent(this->_parent);
    CApp::EffectPool[key].OnLoad("./gfx/explosion.png", 50, 32, 4); 
    CEntity::EntityList.push_back(&CApp::EffectPool[key]);
    PointDouble position( abs( std::min( (int) (this->getPosition().getX() -10 + rand() % 20) ,(int) this->getPosition().getX() )) ,abs( std::min( (int) (this->getPosition().getY() -10 + rand() % 20), (int) this->getPosition().getY() )) );
    CApp::EffectPool[key].setPosition(position, true);
  }
  */
}
