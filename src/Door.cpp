#include "Door.h"
#include "CApp.h"

void Door::OnInit() {
    e->_animControl.Oscillate = true;
    e->_currentFrameCol = 0;
    e->_animControl.MinFrames = 0;
    e->_animControl.MaxFrames = 0;
    e->_animControl.SetFrameRate(10);
}


void Door::OnRender() {
  if(_isOpening) {
    if(e->_animControl.signal) {
      ++_iterationNb;
      e->_animControl.signal = false;
    }
    if(_iterationNb >= _maxIterationNb) { 
      _isOpen = true;
      e->_mask.setHeight(16);
      _isClose = false;
      _isOpening = false;
      _iterationNb = 0;
    }
  }
  if(_isClosing) {
    if(e->_animControl.signal) {
      ++_iterationNb;
      e->_animControl.signal = false;
    }
    if(_iterationNb >= _maxIterationNb) { 
      _isOpen = false;
      e->_mask.setHeight(64);
      _isClose = true;
      _isClosing = false;
      _iterationNb = 0;
    }
  }

}


void Door::OnCleanup() {
  //CEntity::OnCleanup();
  CApp::DoorPool.erase(e->_entityId); 
}


void Door::OnTriggeredAction(int id) {

  if ( id == 2 ) {
    if(_isOpening || _isClosing) { return; }

    if(_isOpen) { Close(); }
    else { Open(); }
  }else if (id == OPEN_DOOR) {
    if(_isClose && !_isOpening) { Open(); }
  }else if (id == CLOSE_DOOR) {
    // Close();
    if(_isOpen && !_isClose) { Close(); }
  }

}

void Door::Open() {
  std::cout << "door open" << std::endl;
  _isOpening = true;
  _isClose = false;
}

void Door::Close() {
  std::cout << "door close" << std::endl;
  // e->_animControl.SetCurrentFrame(e->_animControl.MaxFrames -1);
  _isClosing = true;
  _isOpen = false;
}

void Door::OnAnimate() {
  if(_isClose) {
    e->_currentFrameCol = 0;
    e->_animControl.MinFrames = 0;
    e->_animControl.MaxFrames = 0;
  }else if (_isOpening) {
    e->_currentFrameCol = 0;
    e->_animControl.MinFrames = 0;
    e->_animControl.MaxFrames = 3;
  }else if (_isClosing) {
    e->_currentFrameCol = 0;
    e->_animControl.MinFrames = 0;
    e->_animControl.MaxFrames = 3;
  }else if(_isOpen){
    e->_currentFrameCol = 0;
    e->_animControl.MinFrames = 3;
    e->_animControl.MaxFrames = 3;
  }
}
