
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
  // CApp::BombPool.erase(e->_entityId); 
}

void Bomb::explode(std::vector<CEntity*>& iSheeps) {

  int currentTime = SDL_GetTicks();
  if(currentTime - _startTime < _delay) {
    // bomb did not explode yet
    return;
  }

  // slightly dirty loop
  /*if(_hasExploded) { return; }
  for(std::vector<CEntity*>::iterator itSheep = iSheeps.begin();
      itSheep != iSheeps.end(); ) {
    PointDouble dist;
    CEntity::dist( *e, **itSheep, dist);
    if(dist.modulus() < _radius ) {
      // sheep died in explosion
      int sheepEntityId = (*itSheep)->getEntityId();
      (*itSheep)->removeAtNextLoop() = true;
      itSheep = iSheeps.erase(itSheep);
    }else{
      ++itSheep;
    }
  }*/

  _hasExploded = true;
  e->_removeAtNextLoop = true;

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
