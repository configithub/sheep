#include "CApp.h"
#include <iostream>
#include <math.h>
#include <algorithm>


/*bool CApp::CheckEveryEntityCollision() {
  bool anyCollision = false;



  // report all collisions into CEntityCol::EntityColList
  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
    if(*itEntity == NULL) {
      continue;
    }

    bool collision = !(*itEntity)->PosValidOnEntities((int)((*itEntity)->X), (int)((*itEntity)->Y));
    anyCollision = anyCollision || collision;

  }
  return anyCollision;
}*/

bool CApp::CheckEveryEntityCollision() {

    // report all collisions into CEntityCol::EntityColList
    for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
        if(*itEntity == NULL) {
            continue;
        }

        if( !(*itEntity)->PosValidOnEntities((int)((*itEntity)->X), (int)((*itEntity)->Y)) )
            return false;

    }
    return true;
}

// move all entity, report all collision, then resolve the list of collision even (for loop)
void CApp::OnLoop() {

    // switch time on and monitor framerate
    CFPS::FPSControl.OnLoop();
    MultitouchEvent::Controller.OnLoop(CFPS::FPSControl.getFrameDuration());

    // time between two loops
    float dt = CFPS::FPSControl.GetSpeedFactor();


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
        (*itEntity)->OnLoopMotionBounds(dt);
    }

    // fill collision container by checking every speculative collisions
    for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
        if((*itEntity) == NULL) continue;
        (*itEntity)->PosValidOnEntities((int)((*itEntity)->NextX), (int)((*itEntity)->NextY));
    }

    for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
        if((*itEntity) == NULL) continue;
        (*itEntity)->OnSpeculateMove((*itEntity)->SpeedX, (*itEntity)->SpeedY, dt);
    }

    Solve(3, dt);


    // fill collision container by checking every speculative collisions
    for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
        if((*itEntity) == NULL) continue;
        (*itEntity)->OnLoopRealizeMotion(dt);
    }

    //CheckEveryEntityCollision();

    int nbCollisionCheck = CEntityCol::EntityColList.size();;
    //while(CheckEveryEntityCollision()) {
    bool allCollisionResolved = false;
    /*for(std::map<std::pair<int, int>, CEntityCol>::iterator itEntityCol = CEntityCol::EntityColList.begin();
            itEntityCol != CEntityCol::EntityColList.end(); ++itEntityCol) {


        CEntity* EntityA = itEntityCol->second.EntityA;
        CEntity* EntityB = itEntityCol->second.EntityB;


        if(EntityA == NULL || EntityB == NULL) continue;

        if(EntityA->OnCollision(EntityB, &(itEntityCol->second), nbCollisionCheck, nbCollisionCheck)) {
            EntityB->OnCollision(EntityA, &(itEntityCol->second), nbCollisionCheck, nbCollisionCheck);
        }


    }*/

    CEntityCol::EntityColList.clear();
}

float sign(float value) {
  if(value >= 0) return 1;
  else return -1;
}


void CApp::Solve(int numIterations, float& dt) {

    int numSolved = 0;

    const double kFriction = 0.0;

    for (int j=0; j<numIterations; j++)
    {

        /*for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
          if((*itEntity) == NULL) continue;
          float velX = (*itEntity)->NextX - (*itEntity)->X;
          float velY = (*itEntity)->NextY - (*itEntity)->Y;
          //(*itEntity)->OnSpeculateMove(velX,velY,dt);
          (*itEntity)->ApplyMap(dt);
        }*/

        //std::cout << "iteration: " << j << " number of collision to solve: " << CEntityCol::EntityColList.size() << std::endl;

        for (std::map<std::pair<int, int>, CEntityCol>::iterator itEntityCol = CEntityCol::EntityColList.begin();
                itEntityCol != CEntityCol::EntityColList.end(); ++itEntityCol) {

            CEntityCol& contact = itEntityCol->second;
            contact.updateContactSize();

            CEntity& EntityA = *(contact.EntityA);
            CEntity& EntityB = *(contact.EntityB);



            if(!EntityA.GotoCommand || !EntityB.GotoCommand) {
              EntityA.GotoCommand = false;
              EntityA.StopMove();
              EntityB.GotoCommand = false;
              EntityB.StopMove();

            }


            if(contact.Width >= contact.Height) { // vertical collision

              // TODO tangential

              // normal composant
              //EntityA.NextY += (sign(EntityA.NextY - EntityB.NextY) * contact.Height)/2;
              //EntityB.NextY -= (sign(EntityA.NextY - EntityB.NextY) * contact.Height)/2;

              //EntityA.OnSpeculateMove( EntityA.SpeedX, EntityA.SpeedY + (sign(EntityA.NextY - EntityB.NextY) * contact.Height)/2, dt);
              EntityA.OnSpeculateMove(0, (sign(EntityA.NextY - EntityB.NextY) * contact.Height)/2, dt, true);
              //EntityB.OnSpeculateMove( EntityB.SpeedX, EntityB.SpeedY -(sign(EntityA.NextY - EntityB.NextY) * contact.Height)/2, dt);
              EntityB.OnSpeculateMove(0, -(sign(EntityA.NextY - EntityB.NextY) * contact.Height)/2, dt, true);

            } else { // latteral collision

              // normal composant
              //EntityA.NextX += (sign(EntityA.NextX - EntityB.NextX) * contact.Width)/2;
              //EntityB.NextX -= (sign(EntityA.NextX - EntityB.NextX) * contact.Width)/2;

              EntityA.OnSpeculateMove( (sign(EntityA.NextX - EntityB.NextX) * contact.Width)/2, 0 ,dt);
              EntityB.OnSpeculateMove( -(sign(EntityA.NextX - EntityB.NextX) * contact.Width)/2, 0 ,dt);

            }


        }





    }

    //Debug.WriteLine(numSolved);
}


// treat only first collision then relaunch collision check (while loop)
/*
void CApp::OnLoop() {

  // switch time on and monitor framerate
  CFPS::FPSControl.OnLoop();

  //std::cout << "### STARTING MAIN LOOP ###" << std::endl;

  for(std::vector<CEntity*>::iterator itEntity = CEntity::EntityList.begin(); itEntity != CEntity::EntityList.end(); ++itEntity) {
    if((*itEntity) == NULL) continue;
    (*itEntity)->OffsetX = (*itEntity)->X;
    (*itEntity)->OffsetY = (*itEntity)->Y;
    //(*itEntity)->displayOnTopTree();
    (*itEntity)->OnLoop();
    (*itEntity)->OffsetX -= (*itEntity)->X;
    (*itEntity)->OffsetY -= (*itEntity)->Y;
  }

  //std::cout << "### STARTING COLLISION RESOLUTION ###" << std::endl;

  int nbCollisionCheck = 0;
  //while(CheckEveryEntityCollision()) {
  bool allCollisionResolved = false;
  while(!allCollisionResolved) {
    CheckEveryEntityCollision();
    if(!CEntityCol::EntityColList.empty()) {

      int nbCollision = CEntityCol::EntityColList.size();

      //std::cout << "number of collisions to resolve: " << nbCollision << std::endl;
      //Collision Events

      std::vector<CEntityCol>::iterator itEntityCol = CEntityCol::EntityColList.begin();

      //for(std::vector<CEntityCol>::iterator itEntityCol = CEntityCol::EntityColList.begin(); itEntityCol != CEntityCol::EntityColList.end(); ++itEntityCol) {
        CEntity* EntityA = itEntityCol->EntityA;
        CEntity* EntityB = itEntityCol->EntityB;


        if(EntityA == NULL || EntityB == NULL) continue;

        if(EntityA->OnCollision(EntityB, &(*itEntityCol), nbCollisionCheck, nbCollision)) {
          EntityB->OnCollision(EntityA, &(*itEntityCol), nbCollisionCheck, nbCollision);
        }
        //break;
      //}


    }else{
      allCollisionResolved = true;
    }

    CEntityCol::EntityColList.clear();
    ++nbCollisionCheck;

  }

  CEntityCol::EntityColList.clear();
}
*/
