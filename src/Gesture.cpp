#include "Gesture.h"

//GestureController Gesture::GestureControllerInstance();

GestureController::GestureController() {
  _currentPosition = PointDouble(-1, -1);
  _offset = 16;
  _rawPositions.reserve(40);
  _state = VOID;
  _survivalDelay = 20; // in ms
  _currentDelay = 0;
}

void GestureController::reset() {
  _rawPositions.clear();
  _currentPosition = PointDouble(-1, -1);
  _state = VOID;
}


void GestureController::update(PointDouble& iPosition) {

  if(_currentPosition.getX() == -1 && _currentPosition.getY() == -1) {
    _currentPosition = iPosition;
    _rawPositions.push_back(_currentPosition);
    _state = INCOMING;
  }else{
    PointDouble dist; distance(iPosition, _currentPosition, dist);
    if(dist.getX() > _offset || dist.getY() > _offset) {
      _currentPosition = iPosition;
      _rawPositions.push_back(_currentPosition);
    }
  }

}

void GestureController::OnLoop(bool hasGotMouseEvent, PointDouble& mouse, int& loopDuration) {
  if(hasGotMouseEvent) {
    update(mouse);
    _currentDelay = _survivalDelay;
  }else{
    if(_state == INCOMING) { 
      _currentDelay -= loopDuration;
      if(_currentDelay < 0) { 
        _state = READY_FOR_HARVEST; 
      }
    }
  }
}


void GestureController::harvest(std::vector<PointDouble>& ioPositions, int nbPoints) {
  if(_state == READY_FOR_HARVEST) {
    ioPositions.swap(_rawPositions);
    reset();
  }
}
/*
   void GestureController::harvest(std::vector<PointDouble>& ioPositions, int nbPoints) {
   if(_state == READY_FOR_HARVEST) {
   if(nbPoints == 0) {
   ioPositions.swap(_rawPositions);
   }else{
   int nbRawPoints = _rawPositions.size();
   if(nbPoints == nbRawPoints) {
   ioPositions.swap(_rawPositions);
   }else{
   std::vector<PointDouble> results;
   std::vector<PointDouble> dists;
   std::vector<double> distScalars;
   double totalDist = 0;
   for(std::vector<PointDouble>::iterator itRawPos = _rawPositions.begin(); 
   itRawPos != _rawPositions.end()-1; ++itRawPos) { 
   std::vector<PointDouble>::iterator itNextRawPos = itRawPos +1;
   PointDouble rawDist; distance(*itRawPos, *itNextRawPos, rawDist);
   double rawDistScalar = rawDist.module();
   totalDist+= rawDistScalar;
   dists.push_back(rawDist);
   distScalars.push_back(rawDistScalar);
   }  
   double internode = totalDist / (nbPoints +1);
   int j = 0;
   for (int i = 0; i < nbPoints; i++) {
   double currentInternode = internode;
   bool noMoreRawPoints = false;
   while(currentInternode > distScalars[j]) {
   ++j; if( j >= distScalars.size()) { noMoreRawPoints = true; break; }
   currentInternode -= distScalars[j];
   } 
   if(noMoreRawPoints) { break; }
   if(currentInternode <= distScalars[j]) {
   PointDouble pos = dists[j] * (currentInternode / distScalars[j]) + _rawPositions[j];
   results.push_back(pos);

   }
   }
   }
   }
   reset();
   }
   }*/

/*
   std::vector<PointDouble>:: iterator itRawDist = dists.begin();
   std::vector<PointDouble>:: iterator itRawPos = _rawPositions.begin();
   std::vector<double>:: iterator itDistScalar = distScalars.begin();
   double remainder = 0;
   for(; itRawDist != dists.end(), itDistScalar != distScalars.end(), itRawPos != _rawPositions.end(); ++itRawDist, ++itDistScalar, ++itRawPos) {
   double& distScalar = *itDistScalar;
   PointDouble& rawDist = *itRawDist;
   PointDouble& rawPos = *itRawPos;
   if(remainder > distScalar) { remainder -= distScalar; continue; }else{ distScalar -= remainder; }
   if( internode <= distScalar) {
   while(internode <= distScalar) {
   PointDouble pos = rawDist * (internode / distScalar) + rawPos;
   results.push_back(pos);
   distScalar -= internode;
   } 
   }
   remainder = -distScalar;
   }*/
