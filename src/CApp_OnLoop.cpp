#include "CApp.h"
#include <iostream>
#include <math.h>
#include <algorithm>


// move all entity, report all collision, then resolve the list of collision even (for loop)
void CApp::OnLoop() {

  // switch time on and monitor framerate
  CFPS::FPSControl.OnLoop();
  MultitouchEvent::Controller.OnLoop(CFPS::FPSControl.getFrameDuration());

  // time between two loops
  double dt = CFPS::FPSControl.GetSpeedFactor();

  // handle translation between two rooms
  CCamera::CameraControl.translate(*_nextCenter, dt);


  // handle entity disappearance and maintenance 
  std::vector<CEntity*> newVect;
  CEntity::NextEntityList = newVect; 
  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); 
    itEntity != CEntity::EntityList.end(); ++itEntity) { 
    if( (*itEntity) != NULL) {
      (*itEntity)->OnLoop();
    }
  }  


  // next entities become current entities 
  CEntity::EntityList.swap(CEntity::NextEntityList);

  // apply gravity and controls on all entities
  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    
    (*itEntity)->OnLoopApplyControls(dt);
    //(*itEntity)->OnLoopDeriveAndCapSpeed(dt);
  }

  // apply gravity and controls on all entities
  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->OnLoopMotionBounds();
  }

  // move entities 
  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->OnMove((*itEntity)->getSpeed(), dt);
  }

  SolveCollisions(5, dt);

  // fill collision container by checking every speculative collisions
  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->OnLoopRealizeMotion();
  }

  CEntityCol::EntityColList.clear();
  // delete entity that disappeared during this loop
  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityListToRemove.begin(); 
    itEntity != CEntity::EntityListToRemove.end(); ++itEntity) { 
    (*itEntity)->OnCleanup(); 
  }  
  CEntity::EntityListToRemove.clear(); 
}

double sign(double value) {
  if(value >= 0) return 1;
  else return -1;
}


void CApp::updateScore() {
  int currentTime = SDL_GetTicks();
  if(currentTime - scoreTimer > 1000) {
    scoreTimer = currentTime;
    score += Sheeps.size()*5;
  }
}

void CApp::SolveCollisions(int numIterations, double& dt) {



  for (int j=0; j<numIterations; j++) {

    // fill collision container by checking every speculative collisions
    CEntityCol::EntityColList.clear();
    for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
      if((*itEntity) == NULL) continue;
      (*itEntity)->PosValidOnEntities();
    }
    
    if(CEntityCol::EntityColList.empty()) { return; }

    for (std::map<std::pair<int, int>, CEntityCol>::iterator itEntityCol = CEntityCol::EntityColList.begin();
        itEntityCol != CEntityCol::EntityColList.end(); ++itEntityCol) {

      CEntityCol& contact = itEntityCol->second;

      contact.updateContactSize();
      if(contact._rectangle.isVoid()) { continue; }
      CEntity& EntityA = *(contact.EntityA);
      CEntity& EntityB = *(contact.EntityB);

      // SAW kills SHEEP
      if(EntityB.getType() == SHEEP && EntityA.getType() == SAW) {
        EntityB.b->kill();
        continue;
      } 
      if (EntityA.getType() == SHEEP && EntityB.getType() == SAW) {
        EntityA.b->kill();
        continue;
      } 

      // SHEEP triggers SWITCH
      if(EntityB.getType() == SHEEP && EntityA.getType() == SWITCH) {
        EntityA.b->OnTriggeredAction(1);
        continue;
      } 

      if(contact._rectangle.getWidth() >= contact._rectangle.getHeight()) { // vertical collision

        if(EntityA.getType() == EntityB.getType() ) {
          PointDouble repulsA = PointDouble(0, (sign(EntityA.getNextPosition().getY()
                               - EntityB.getNextPosition().getY()) * contact._rectangle.getHeight())/2);
          PointDouble repulsB = PointDouble(0, -(sign(EntityA.getNextPosition().getY()
                               - EntityB.getNextPosition().getY()) * contact._rectangle.getHeight())/2);
          EntityA.OnMove(repulsA, dt);
          EntityB.OnMove(repulsB, dt);
        }else if (EntityA.getType() > EntityB.getType() ) { // A heavier than B : A does not move
          PointDouble repulsB = PointDouble(0, -(sign(EntityA.getNextPosition().getY()
                               - EntityB.getNextPosition().getY()) * contact._rectangle.getHeight()));
          EntityB.OnMove(repulsB, dt);
        }else if (EntityB.getType() > EntityA.getType() ) { // B heavier than A : B does not move
          PointDouble repulsA = PointDouble(0, -(sign(EntityB.getNextPosition().getY()
                               - EntityA.getNextPosition().getY()) * contact._rectangle.getHeight()));
          EntityA.OnMove(repulsA, dt);
        }

      } else { // latteral collision
        if(EntityA.getType() == EntityB.getType() ) {
          PointDouble repulsA = PointDouble((sign(EntityA.getNextPosition().getX() 
                                - EntityB.getNextPosition().getX()) * contact._rectangle.getWidth())/2, 0);
          PointDouble repulsB = PointDouble(-(sign(EntityA.getNextPosition().getX()
                                - EntityB.getNextPosition().getX()) * contact._rectangle.getWidth())/2, 0);
          EntityA.OnMove( repulsA, dt);
          EntityB.OnMove( repulsB, dt);
        }else if (EntityA.getType() > EntityB.getType() ) { // A heavier than B : A does not move
          PointDouble repulsB = PointDouble(-(sign(EntityA.getNextPosition().getX()
                                - EntityB.getNextPosition().getX()) * contact._rectangle.getWidth()), 0);
          EntityB.OnMove( repulsB, dt);
        }else if (EntityB.getType() > EntityA.getType() ) { // B heavier than A : B does not move
          PointDouble repulsA = PointDouble((sign(EntityA.getNextPosition().getX() 
                                - EntityB.getNextPosition().getX()) * contact._rectangle.getWidth()), 0);
          EntityA.OnMove( repulsA, dt);
        }

      }
    }

  }

}

