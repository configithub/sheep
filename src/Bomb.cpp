
#include "Bomb.h"
#include "CApp.h"
#include "Effect.h"


void Bomb::OnRender(SDL_Surface* Surf_Display) {
  int remaining = std::max(0, (int) (_delay - (SDL_GetTicks() - _startTime)) /1000);
  // CApp::print_num(Surf_Display, CApp::Surf_NumFontBomb, e->_position.getX(), e->_position.getY(), remaining ); 
}

void Bomb::OnLoop() {
  explode(CApp::Sheeps);  
}

void Bomb::OnAnimate() {
  // give correct entity orientation
  if(e->_moveLeft) {
    e->_currentFrameCol = 0;
  }else if(e->_moveRight) {
    e->_currentFrameCol = 1;
  }

  e->_animControl.MinFrames = 0;
  e->_animControl.MaxFrames = 2;
  e->_animControl.Oscillate = true;

}

void Bomb::OnCleanup() {
  CApp::BombPool.erase(e->_entityId); 
}

void Bomb::explode(std::vector<Entity*>& iSheeps) {
  int currentTime = SDL_GetTicks();
  if(currentTime - _startTime < _delay) {
    // bomb did not explode yet
    return;
  }
  std::cout << "bomb is exploding: " << e->_entityId << std::endl;
  // slightly dirty loop
  if(_hasExploded) { return; }
  for(std::vector<Entity*>::iterator itSheep = iSheeps.begin();
      itSheep != iSheeps.end(); ++itSheep ) {
    PointDouble dist;
    Entity::dist( *e, **itSheep, dist);
    if(dist.modulus() < _radius ) {
      // sheep died in explosion
      (*itSheep)->b->kill();
    }
  }
  _hasExploded = true;
  e->_removeAtNextLoop = true;
  // explosion effect
  int nbExplosionEffect = rand() % 5+3;
  for (int i = 0; i < nbExplosionEffect; i++) {
    int key = Entity::CurrentEntityId;
    PointDouble position( abs( std::min( (int) (this->e->getPosition().getX() -10 + rand() % 20) ,(int) this->e->getPosition().getX() )) ,abs( std::min( (int) (this->e->getPosition().getY() -10 + rand() % 20), (int) this->e->getPosition().getY() )) );
    CApp::EntityPool[key].generateAtPos(position, EFFECT, e->_parent); // CORE
  }
 
}
