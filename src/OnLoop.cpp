#include "CApp.h"
#include <iostream>
#include <math.h>
#include <algorithm>


// move all entity, report all collision, then resolve the list of collision even (for loop)
void CApp::OnLoop() {

  // switch time on and monitor framerate
  MultitouchEvent::Controller.OnLoop(FPS::FPSControl.getFrameDuration());

  // time between two loops
  double dt = FPS::FPSControl.GetSpeedFactor();
  //int frameDuration = FPS::FPSControl.getFrameDuration();
  //std::cout << "dt: " << dt << std::endl;
  //std::cout << "frameDuration: " << frameDuration << std::endl;

  // continuous translation : 
  if(!Camera::CameraControl.isTranslating()) {
    _nextCenter->set(_center->getX()+WWIDTH, _center->getY());
  }

  // handle translation between two rooms
  Camera::CameraControl.translate(*_nextCenter, _translationSpeed, dt); // true => continuous translation

  // handle entity disappearance and maintenance 
  std::vector<Entity*> newVect;
  Entity::NextEntityList = newVect; 
  for(std::vector<Entity*>::iterator itEntity = Entity::EntityList.begin(); 
    itEntity != Entity::EntityList.end(); ++itEntity) { 
    if( (*itEntity) != NULL) {
      (*itEntity)->OnLoop();
    }
  }  

  // next entities become current entities 
  Entity::EntityList.swap(Entity::NextEntityList);

  // adjust position for entities that are translating along with the maps
  for(std::vector<Entity*>::iterator itEntity = Entity::EntityList.begin(); itEntity != Entity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->adjustForTranslation(Camera::CameraControl.getMoveX(), Camera::CameraControl.getMoveY(), dt);
  }

  // apply gravity and controls on all entities
  for(std::vector<Entity*>::iterator itEntity = Entity::EntityList.begin(); itEntity != Entity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    
    (*itEntity)->OnLoopApplyControls(dt);
    //(*itEntity)->OnLoopDeriveAndCapSpeed(dt);
  }

  // apply gravity and controls on all entities
  for(std::vector<Entity*>::iterator itEntity = Entity::EntityList.begin(); itEntity != Entity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->OnLoopMotionBounds();
  }

  // move entities 
  for(std::vector<Entity*>::iterator itEntity = Entity::EntityList.begin(); itEntity != Entity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->OnMove((*itEntity)->getSpeed(), dt);
  }

  SolveCollisions(5, dt);

  // fill collision container by checking every speculative collisions
  for(std::vector<Entity*>::iterator itEntity = Entity::EntityList.begin(); itEntity != Entity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->OnLoopRealizeMotion();
  }

  EntityCol::EntityColList.clear();
  // delete entity that disappeared during this loop
  for(std::vector<Entity*>::iterator itEntity = Entity::EntityListToRemove.begin(); 
    itEntity != Entity::EntityListToRemove.end(); ++itEntity) { 
    (*itEntity)->OnCleanup(); 
  }  
  Entity::EntityListToRemove.clear(); 
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
    EntityCol::EntityColList.clear();
    for(std::vector<Entity*>::iterator itEntity = Entity::EntityList.begin(); itEntity != Entity::EntityList.end(); ++itEntity) {
      if((*itEntity) == NULL) continue;
      (*itEntity)->PosValidOnEntities();
    }
    
    if(EntityCol::EntityColList.empty()) { return; }

    for (std::map<std::pair<int, int>, EntityCol>::iterator itEntityCol = EntityCol::EntityColList.begin();
        itEntityCol != EntityCol::EntityColList.end(); ++itEntityCol) {

      EntityCol& contact = itEntityCol->second;

      contact.updateContactSize();
      if(contact._rectangle.isVoid()) { continue; }
      Entity& EntityA = *(contact.EntityA);
      Entity& EntityB = *(contact.EntityB);

      // SAW kills SHEEP
      if(EntityB.getType() == SHEEP && EntityA.getType() == SAW) {
        EntityB.b->kill();
        continue;
      } 
      if (EntityA.getType() == SHEEP && EntityB.getType() == SAW) {
        EntityA.b->kill();
        continue;
      } 

      // ACTIVE SHEEP TOUCHES INACTIVE SHEEP
      if(EntityB.getType() == SHEEP && EntityA.getType() == SHEEP) {
        if(EntityA.b->getAttribute(PLAYER_CONTROLLED).get_int() == 0) {
          std::cout << "activating a sheep" << std::endl;
          EntityA.b->setAttribute(PLAYER_CONTROLLED, 1);
          Sheeps.push_back(&EntityA);
        }
      } 

      // SHEEP triggers SWITCH
      if(EntityB.getType() == SHEEP && EntityA.getType() == SWITCH) {
        EntityA.b->OnTriggeredAction(TRIGGER_ON_COLLISION);
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

