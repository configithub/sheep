#include "Door.h"
#include "CApp.h"

void Door::OnInit() {
    e->_animControl.Oscillate = true;
    e->_currentFrameCol = 0;
    e->_animControl.MinFrames = 0;
    e->_animControl.MaxFrames = 0;
    e->_animControl.SetFrameRate(10);
}

/*
void Door::OnRender() {
  if(_doorState == OPENING) {
    if(e->_animControl.signal) {
      ++_iterationNb;
      e->_animControl.signal = false;
    }
    if(_iterationNb >= _maxIterationNb) { 
      _doorState = OPEN;
      _iterationNb = 0;
    }
  }
  if(_doorState == CLOSING) {
    if(e->_animControl.signal) {
      ++_iterationNb;
      e->_animControl.signal = false;
    }
    if(_iterationNb >= _maxIterationNb) { 
      _doorState = CLOSED;
      _iterationNb = 0;
    }
  }
}*/

void Door::OnRender() {
  if(_doorState == OPENING) { 
    if(e->_animControl.signal) { // opening animation ended
      e->_animControl.signal = false;
      _doorState = OPEN;
    }
  }
  if(_doorState == CLOSING) {
    if(e->_animControl.signal) { // closing animation ended
      e->_animControl.signal = false;
      _doorState = CLOSED;
    }
  }
}


void Door::OnCleanup() {
  CApp::DoorPool.erase(e->_entityId); 
}


void Door::OnTriggeredAction(int id) {

  if ( id == CHANGE_DOOR_STATE ) {
    if(_doorState == OPENING || _doorState == CLOSING) { return; }
    else if(_doorState == OPEN) { Close(); }
    else if(_doorState == CLOSED) { Open(); }
  }else if (id == OPEN_DOOR) {
    if(_doorState != OPEN && _doorState != OPENING) { Open(); }
  }else if (id == CLOSE_DOOR) {
    if(_doorState != CLOSED && _doorState != CLOSING) { Close(); }
  }

}

void Door::Open() {
  std::cout << "door open" << std::endl;
  _doorState = OPENING;
  e->_mask.setHeight(16);
  e->_animControl.SetFrameInc(1);
}

void Door::Close() {
  std::cout << "door close" << std::endl;
  _doorState = CLOSING;
  e->_mask.setHeight(64);
  e->_animControl.SetFrameInc(-1);
}

void Door::OnAnimate() {
  switch(_doorState) {
    case CLOSED:
    e->_currentFrameCol = 0;
    e->_animControl.MinFrames = 0;
    e->_animControl.MaxFrames = 0;
    break;
    case CLOSING:
    e->_currentFrameCol = 0;
    e->_animControl.MinFrames = 0;
    e->_animControl.MaxFrames = 3;
    break;
    case OPEN:
    e->_currentFrameCol = 0;
    e->_animControl.MinFrames = 3;
    e->_animControl.MaxFrames = 3;
    break;
    case OPENING:
    e->_currentFrameCol = 0;
    e->_animControl.MinFrames = 0;
    e->_animControl.MaxFrames = 3;
    break;
  }
}
