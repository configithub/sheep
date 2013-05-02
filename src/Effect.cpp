#include "Effect.h"
#include "CFPS.h"
#include <iostream>
#include "CApp.h"


void Effect::OnRender(SDL_Surface* Surf_Display) {
  if(e->_animControl.signal) {
    ++_iterationNb;
    e->_animControl.signal = false;
  }
  if(_iterationNb >= _maxIterationNb) { 
    e->_removeAtNextLoop = true;
  }
}

void Effect::OnCleanup() {
  //CEntity::OnCleanup();
  //CApp::EffectPool.erase(_entityId); 
}

void Effect::OnAnimate() {
  e->_currentFrameCol = e->_entityId%3;
  e->_animControl.MinFrames = 0;
  e->_animControl.MaxFrames = 3;
  e->_animControl.SetFrameRate(70); //nb of ms for each frame

}




