#include "CFollower.h"
#include "CApp.h"

#include <iostream>


CFollower::CFollower() {}

void CFollower::kill() {
  e->_removeAtNextLoop = true;
}

void CFollower::OnCleanup() {
  CApp::SheepPool.erase(e->_entityId); 
  std::cout << "killing a sheep" << std::endl;
  for(std::vector<CEntity*>::iterator itSheep = CApp::Sheeps.begin(); 
    itSheep != CApp::Sheeps.end(); ++itSheep) { 
    if(e->_entityId == (*itSheep)->_entityId){
      itSheep = CApp::Sheeps.erase(itSheep);
      break;
    }
  }  
  
}

void CFollower::OnAnimate() {

  // give correct entity orientation
  if(e->_moveLeft) {
    e->_currentFrameCol = 0;
    if(e->_tileType == TILE_TYPE_GRASS) {
      e->_currentFrameCol+=2;
    }
  }else if(e->_moveRight) {
    e->_currentFrameCol = 1;
    if(e->_tileType == TILE_TYPE_GRASS) {
      e->_currentFrameCol+=2;
    }
  }

  e->_animControl.MinFrames = 1;
  if(e->_moveRight || e->_moveLeft
       || e->_moveUp || e->_moveDown) {
    e->_animControl.MaxFrames = 3;
  } else {
    e->_animControl.MaxFrames = 1;
  }

  if(e->_tileType == TILE_TYPE_GRASS) {
    if(!e->_moveRight && !e->_moveLeft
         && !e->_moveUp && !e->_moveDown) {
      e->_animControl.MinFrames = 0;
      e->_animControl.MaxFrames = 1;
    }
  }

}




