#include "CApp.h"
#include <iostream>
#include <math.h>
#include <algorithm>
#include "Level.h"


// move all entity, report all collision, then resolve the list of collision even (for loop)
void CApp::OnLoop() {

  // switch time on and monitor framerate
  CFPS::FPSControl.OnLoop();
  MultitouchEvent::Controller.OnLoop(CFPS::FPSControl.getFrameDuration());

  // time between two loops
  double dt = CFPS::FPSControl.GetSpeedFactor();

  // todo, class for this
  CEntity::EntityList.clear();
  CEntity::EntityList.reserve(100);
  std::vector<int> entitiesToRemove; 
  
  for(std::map<int,CFollower>::iterator itSheep = EntityPool.begin();
    itSheep != EntityPool.end(); ++itSheep) {
    if(!itSheep->second.removeAtNextLoop()) {
      CEntity::EntityList.push_back(&(itSheep->second));
    }else{
      entitiesToRemove.push_back(itSheep->second.getEntityId());
    }
  }
  for(std::vector<int>::iterator itId = entitiesToRemove.begin(); 
    itId != entitiesToRemove.end(); ++itId) { 
    EntityPool.erase(*itId);
  }  
  entitiesToRemove.clear(); 
  for(std::map<int,Effect>::iterator itEffect = CApp::EffectPool.begin(); 
    itEffect != CApp::EffectPool.end(); ++itEffect) { 
    if(!itEffect->second.removeAtNextLoop()) {
      CEntity::EntityList.push_back(&(itEffect->second));
    }else{
      entitiesToRemove.push_back(itEffect->second.getEntityId());
    }
  } 
  for(std::vector<int>::iterator itId = entitiesToRemove.begin(); 
    itId != entitiesToRemove.end(); ++itId) { 
    EffectPool.erase(*itId);
  }  
  entitiesToRemove.clear();
  for(std::map<int,Bomb>::iterator itBomb = Level::LevelInstance.getBombs().begin(); 
    itBomb != Level::LevelInstance.getBombs().end(); ++itBomb) { 
    if(!itBomb->second.hasExploded()) {
      CEntity::EntityList.push_back(&(itBomb->second));
    }else{
      entitiesToRemove.push_back(itBomb->second.getEntityId());
    }
  } 
  for(std::vector<int>::iterator itId = entitiesToRemove.begin(); 
    itId != entitiesToRemove.end(); ++itId) { 
    Level::LevelInstance.getBombs().erase(*itId);
  }  
  entitiesToRemove.clear();
    

  // apply gravity and controls on all entities
  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    //(*itEntity)->OnLoopApplyGravity();
    (*itEntity)->OnLoopApplyControls();
    (*itEntity)->OnLoopDeriveAndCapSpeed(dt);
  }

  // apply gravity and controls on all entities
  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->OnLoopMotionBounds();
  }

  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->OnMove((*itEntity)->getSpeed(), dt);
  }

  SolveCollisions(3, dt);

  // fill collision container by checking every speculative collisions
  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->OnLoopRealizeMotion();
  }

  // update level information
  Level::LevelInstance.OnLoop(Sheeps);
  if( Level::LevelInstance.isOver() ) {
    if( Level::LevelInstance.isLevelSuccess() ) {
      if(Sheeps.size() <= 30 ) {
        AddNewSheepInPool(activeSheep);
      }
    }
    std::string levelName = "level2";
    if(Level::LevelInstance.getLevelNb() <= 30) {
      Level::LevelInstance.next(Sheeps,levelName, 0, 100, 2500, 2);
    }else if(Level::LevelInstance.getLevelNb() <= 150) {
      Level::LevelInstance.next(Sheeps,levelName, 0, 100, 2000, 2);
    }else if(Level::LevelInstance.getLevelNb() <= 450) {
      Level::LevelInstance.next(Sheeps,levelName, 0, 100, 1500, 2);
    }else{
      Level::LevelInstance.next(Sheeps,levelName, 0, 100, 1000, 2);
    }
  }

  CApp::updateScore();

  CEntityCol::EntityColList.clear();
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

      if(contact._rectangle.isVoid()) { continue; }
      CEntity& EntityA = *(contact.EntityA);
      CEntity& EntityB = *(contact.EntityB);

      //int flip = rand()%2;
      if(contact._rectangle.getWidth() >= contact._rectangle.getHeight()) { // vertical collision
      //if(flip) { // vertical collision
        PointDouble repulsA = PointDouble(0, (sign(EntityA.getNextPosition().getY() - EntityB.getNextPosition().getY()) * contact._rectangle.getHeight())/2);
        PointDouble repulsB = PointDouble(0, -(sign(EntityA.getNextPosition().getY() - EntityB.getNextPosition().getY()) * contact._rectangle.getHeight())/2);
        EntityA.OnMove(repulsA, dt);
        EntityB.OnMove(repulsB, dt);
      } else { // latteral collision
        PointDouble repulsA = PointDouble((sign(EntityA.getNextPosition().getX() - EntityB.getNextPosition().getX()) * contact._rectangle.getWidth())/2, 0);
        PointDouble repulsB = PointDouble(-(sign(EntityA.getNextPosition().getX() - EntityB.getNextPosition().getX()) * contact._rectangle.getWidth())/2, 0);
        EntityA.OnMove( repulsA, dt);
        EntityB.OnMove( repulsB, dt);
      }
        //PointDouble repulsA = PointDouble( (sign(EntityA.getNextPosition().getX() - EntityB.getNextPosition().getX()) * contact._rectangle.getWidth())/2, (sign(EntityA.getNextPosition().getY() - EntityB.getNextPosition().getY()) * contact._rectangle.getHeight())/2);
        //PointDouble repulsB = PointDouble( -(sign(EntityA.getNextPosition().getX() - EntityB.getNextPosition().getX()) * contact._rectangle.getWidth())/2, -(sign(EntityA.getNextPosition().getY() - EntityB.getNextPosition().getY()) * contact._rectangle.getHeight())/2);
        //EntityA.OnMove(repulsA, dt);
        //EntityB.OnMove(repulsB, dt);
      contact.updateContactSize();
    }

  }

}

