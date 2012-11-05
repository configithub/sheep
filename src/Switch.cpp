
#include "Switch.h"
#include "CApp.h"

bool Switch::OnLoad(char* File, int Width, int Height, int MaxFrames) {

  _sdlSurface = CSurface::Sprites[3];
  _type = SWITCH;
  
  Rectangle mask(0,0,Width,Height);
  if(CEntity::OnLoad(File, mask, MaxFrames) == false) {
    return false;
  }

  return true;
}

void Switch::OnRender(SDL_Surface* Surf_Display) {
  if(_isPushed) {
    int currentTime = SDL_GetTicks();
    if(currentTime - _startTime < _delay) {
      // switch did not go back to unpushed state yet 
    }else{
      _isPushed = false;
    }
  }
  CEntity::OnRender(Surf_Display);
}


void Switch::OnAnimate() {

  _currentFrameCol = 0;

  if (!_isPushed) {
    _animControl.MinFrames = 0;
    _animControl.MaxFrames = 0;
  }else{
    _animControl.MinFrames = 1;
    _animControl.MaxFrames = 1;
  }

  CEntity::OnAnimate();

}

// action 0
void Switch::spawnBombInRoom() {
  // get suitable rectangle for the current room
  Rectangle aScreenRect(_center.getX()+33,
      _center.getY()+33, WWIDTH-33, WHEIGHT-33);

  // spawn a bomb randomly in this rectangle
  int key = CEntity::CurrentEntityId;
  CApp::BombPool[key].generateRandom(aScreenRect);

}

void Switch::trigger() {

  if(_isPushed) { return; }
  _isPushed = true;
  _startTime = SDL_GetTicks();   

  if(_actionId == 0) {
    spawnBombInRoom();
  }

}

void Switch::triggerOnTouch(PointDouble& iMouse) { 
  if(_triggerId != 0) { return; }
  PointDouble distance;
  CEntity::dist(*this, iMouse, distance);

  if ( distance.getX() < 32 && distance.getY() < 32 ) {
    trigger();
  }

}


void Switch::generateAtPos(PointDouble& iPosition) {
  _position = iPosition;
  _nextPosition = _position;

  OnLoad("./gfx/switch.png", 32, 32, 4);

  CEntity::EntityList.push_back(this);

}



















