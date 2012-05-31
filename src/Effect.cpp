#include "Effect.h"
#include "CFPS.h"
#include <iostream>


bool Effect::OnLoad(char* File, int Width, int Height, int MaxFrames) {
  
  
  Rectangle mask(0,0,Width,Height);
  if(CEntity::OnLoad(File, mask, MaxFrames) == false) {
    return false;
  }

  return true;
}


void Effect::OnRender(SDL_Surface* Surf_Display) {
  CEntity::OnRender(Surf_Display);
  ++_iterationNb;
  if(_iterationNb >= _maxIterationNb*_animControl.MaxFrames*CFPS::FPSControl.GetFPS()*_animControl.GetFrameRate() ) {
    _removeAtNextLoop = true;
  }
}

void Effect::OnCleanup() {
  CEntity::OnCleanup();
}


void Effect::OnAnimate() {
  _currentFrameCol = rand()%3;
  _animControl.MinFrames = 0;
  _animControl.MaxFrames = 3;
  _animControl.SetFrameRate(10);


}




