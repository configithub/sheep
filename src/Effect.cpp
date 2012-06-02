#include "Effect.h"
#include "CFPS.h"
#include <iostream>


bool Effect::OnLoad(char* File, int Width, int Height, int MaxFrames) {
  
  _sdlSurface = CSurface::Sprites[2];
  
  Rectangle mask(0,0,Width,Height);
  if(CEntity::OnLoad(File, mask, MaxFrames) == false) {
    return false;
  }

  return true;
}


void Effect::OnRender(SDL_Surface* Surf_Display) {
  if(_animControl.signal) {
    ++_iterationNb;
    _animControl.signal = false;
  }
  if(_iterationNb >= _maxIterationNb) { 
    _removeAtNextLoop = true;
  }else{
    CEntity::OnRender(Surf_Display);
  }
}

void Effect::OnCleanup() {
  CEntity::OnCleanup();
}


void Effect::OnAnimate() {
  //_currentFrameCol = rand()%3;
  _currentFrameCol = _entityId%3;
  _animControl.MinFrames = 0;
  _animControl.MaxFrames = 3;
  _animControl.SetFrameRate(70); //nb of ms for each frame

  CEntity::OnAnimate();

}




