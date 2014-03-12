#include "Effect.h"
#include "FPS.h"
#include <iostream>
#include "CApp.h"


void Effect::OnRender() {
  if(e->_animControl.signal) {
    ++_iterationNb;
    e->_animControl.signal = false;
  }
  if(_iterationNb >= _maxIterationNb) { 
    e->_removeAtNextLoop = true;
  }
}

void Effect::OnCleanup() {
  CApp::EffectPool.erase(e->_entityId); 
}

void Effect::OnAnimate() {
  e->_currentFrameCol = e->_entityId%3;
  e->_animControl.MinFrames = 0;
  e->_animControl.MaxFrames = 3;
  e->_animControl.SetFrameRate(70); //nb of ms for each frame

}




