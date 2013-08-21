#include "CApp.h"
#include "Gesture.h"


void CApp::OnGesture() {

  int loopDuration = CFPS::FPSControl.getFrameDuration();
  //std::cout << "_hasGotMouseEventThisLoop: " << _hasGotMouseEventThisLoop << std::endl;
  GestureControllerInstance.OnLoop(_hasGotMouseEventThisLoop, Mouse, loopDuration);

  std::vector<PointDouble> gesturePositions;
  GestureControllerInstance.harvest(gesturePositions);
  if(!gesturePositions.empty()) {
    for(std::vector<PointDouble>::iterator itPos = gesturePositions.begin(); 
      itPos != gesturePositions.end(); ++itPos) { 
      //std::cout << "*itPos: " << *itPos << std::endl;
    }  
  }
  SelectAllSheepsInCurrentRoom();
  GoToGesture(gesturePositions);


}

void CApp::GoToGesture(std::vector<PointDouble>& gesturePositions) {

  int i = 0; int size = gesturePositions.size();
  if(size == 0) { return; }
  for(std::vector<CEntity*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->_selection == activeSheep) {
      (*itSheep)->isTargettingPosition(true);
      (*itSheep)->getTargetPosition().set(gesturePositions[i%size].getX(), gesturePositions[i%size].getY());
      ++i;
    }
  }
}
