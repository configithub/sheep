#include "CApp.h"
#include "Gesture.h"


void CApp::OnGesture() {

  int loopDuration = FPS::FPSControl.getFrameDuration();
  //std::cout << "_hasGotMouseEventThisLoop: " << _hasGotMouseEventThisLoop << std::endl;
  GestureControllerInstance.OnLoop(_hasGotMouseEventThisLoop, Mouse, loopDuration);

  if(MultitouchEvent::Controller.getNumberOfActivePoints() > 1) {
    for(std::vector<PointDouble>::iterator itControl = controls.begin(); 
      itControl != controls.end(); ++itControl) { 
      GestureControllerInstance.OnLoop(_hasGotMouseEventThisLoop, *itControl, loopDuration);
    }  
  }

  std::vector<PointDouble> gesturePositions;
  GestureControllerInstance.harvest(gesturePositions);
  if(!gesturePositions.empty()) {
    SelectAllSheepsInCurrentRoom();
    GoToGesture(gesturePositions);
    // std::cout << "_nbSelectedEntity: " << _nbSelectedEntity << std::endl;
  }


}

/*
void CApp::GoToGesture(std::vector<PointDouble>& gesturePositions) {

  int i = 0; int size = gesturePositions.size();
  if(size == 0) { return; }
  for(std::vector<Entity*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->_selection == activeSheep) {
      (*itSheep)->isTargettingPosition(true);
      (*itSheep)->getTargetPosition().set(gesturePositions[i%size].getX(), gesturePositions[i%size].getY());
      ++i;
    }
  }
}*/

void CApp::GoToGesture(std::vector<PointDouble>& gesturePositions) {
  int size = gesturePositions.size();
  std::vector<bool> taboo;
  taboo.reserve(size);
  for (int j = 0; j < size; j++) {
    taboo.push_back(false);
  }
  if(size == 0) { return; }
  for(std::vector<Entity*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->_selection == activeSheep) {
      (*itSheep)->isTargettingPosition(true);
      int delta = 999999999;
      PointDouble NearestPos; bool allTaboo = true;
      for(std::vector<bool>::iterator itTaboo = taboo.begin(); 
        itTaboo != taboo.end(); ++itTaboo) { if(!(*itTaboo)) { allTaboo = false ; break; } }  
      if(allTaboo) {
        for(std::vector<bool>::iterator itTaboo = taboo.begin(); 
            itTaboo != taboo.end(); ++itTaboo) { *itTaboo = false; }
      }
      int j = 0; int NearestIndex = 0;
      for(std::vector<PointDouble>::iterator itPos = gesturePositions.begin();
          itPos != gesturePositions.end(); ++itPos) {
        if(!taboo[j]) {
          PointDouble distPoint; distance( *itPos, (*itSheep)->getPosition(), distPoint);
          int dist = distPoint.modulus();
          if( dist < delta) {
            delta = dist;
            NearestPos = *itPos;
            NearestIndex = j;
          }
        }
        ++j;
      }
      taboo[NearestIndex] = true;
      (*itSheep)->getTargetPosition().set(NearestPos.getX(), NearestPos.getY());
    }
  }
}
