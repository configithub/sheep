#include "Saw.h"
#include "CApp.h"

void Saw::OnInit() {

  // adjust the frame rate for the saw animation
  e->_animControl.SetFrameRate(1);

}




void Saw::OnRender() { }


void Saw::OnCleanup() {
  //Entity::OnCleanup();
  CApp::SawPool.erase(e->_entityId); 
}


void Saw::OnAnimate() {
  e->_currentFrameCol = 0;
  e->_animControl.MinFrames = 0;
  e->_animControl.MaxFrames = 11;
}
