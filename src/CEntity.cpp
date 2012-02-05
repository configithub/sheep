#include "CEntity.h"
#include <iostream>
#include <math.h>
#include <algorithm>


std::vector<CEntity*>     CEntity::EntityList;
int CEntity::CurrentEntityId = 0;



CEntity::CEntity()
{
  Surf_Entity = NULL;

  // entity coordinates and size
  X = 400;
  Y = 400;

  gotoX =0;
  gotoY =0;
  GotoCommand = false;

  NextX = 400;
  NextY = 400;

  Width     = 0;
  Height     = 0;

  // animate left or right?
  MoveLeft  = false;
  MoveRight = false;

  // entity type
  Type =     ENTITY_TYPE_GENERIC;

  Dead = false;
  Flags = ENTITY_FLAG_GRAVITY;

  // movement parameters

  // speed, pixel * second^-1
  SpeedX = 0;
  SpeedY = 0;

  // acceleration, pixel * second^-2
  AccelX = 0;
  AccelY = 0;

  // max speed, pixel * second^-1
  MaxSpeedX = 10;
  MaxSpeedY = 10;

  // animation tile settings
  CurrentFrameCol = 0;
  CurrentFrameRow = 0;

  // collision mask of the entity
  // values are offsets (i.e. margins)
  Col_X = 0;
  Col_Y = -5;


  Col_Width  = 0;
  Col_Height = 2;


  // entity able to jump?
  CanJump = false;

  CanFollow = false;

  NearestEntity = this;


  // to identify the entity for debugging
  entityId=CEntity::CurrentEntityId;
  ++CEntity::CurrentEntityId;

}



CEntity::~CEntity()
{
}

bool CEntity::OnLoad(char* File, int Width, int Height, int MaxFrames)
{
  if((Surf_Entity = CSurface::OnLoad(File)) == NULL)
  {
    return false;
  }

  this->Width = Width;
  this->Height = Height;

  Anim_Control.MaxFrames = MaxFrames;

  return true;
}

bool CEntity::Jump()
{
  if(CanJump == false)
  {
    //std::cout << "1" << std::endl;
    return false;
  }
  //std::cout << "2" << std::endl;
  SpeedY = -MaxSpeedY;
  return true;
}




// main process for entities, cast inside the main game loop
void CEntity::OnLoop()
{

  /*

  //We're not Moving
  if(MoveLeft == false && MoveRight == false) {
    StopMove(); // set acceleration to the opposite value
    // to gradually stop motion
  }

  if(MoveLeft ) {
    AccelX = -0.5; // gradually increse speed to the left
  } else

    if(MoveRight) {
      AccelX = 0.5; // gradually increse speed to the right
    }

  if(Flags & ENTITY_FLAG_GRAVITY) {
    AccelY = 0.75f; // g factor
  }

  // set speed according to acceleration
  // FPS control is included to be consistent across various system perfs
  // => acceleration is constant in terms of pixel * second^-2 for diff. perfs
  SpeedX += AccelX * CFPS::FPSControl.GetSpeedFactor();
  SpeedY += AccelY * CFPS::FPSControl.GetSpeedFactor();

  // cap the speed
  if(SpeedX > MaxSpeedX)  SpeedX =  MaxSpeedX;
  if(SpeedX < -MaxSpeedX) SpeedX = -MaxSpeedX;
  if(SpeedY > MaxSpeedY)  SpeedY =  MaxSpeedY;
  if(SpeedY < -MaxSpeedY) SpeedY = -MaxSpeedY;



  OnAnimate(); // update entity animation

  // implement speculative contacts

  */


}

// main process for entities, cast inside the main game loop
void CEntity::OnLoopApplyGravity()
{

  if(Flags & ENTITY_FLAG_GRAVITY)
  {
    AccelY = 0.75f; // g factor
  }

}


// orbital goto
/*if(GotoCommand)
  {

    if (gotoX < ( X -50) )  {
      //std::cout << "goto left" << std::endl;
      MoveLeft = true;
      MoveRight = false;

    }
    if(gotoX > X + 50) {
      // std::cout << "goto right" << std::endl;
      MoveRight = true;
      MoveLeft = false;

    }

    if ( X < gotoX+50 && X > gotoX-50 ) {
      //std::cout << "stop" << std::endl;
      MoveLeft = false;
      MoveRight = false;

    }

     if (gotoY < ( Y -50) )  {
      //std::cout << "goto left" << std::endl;
      MoveUp = true;
      MoveDown = false;

    }
    if(gotoY > Y + 50) {
      // std::cout << "goto right" << std::endl;
      MoveDown = true;
      MoveUp = false;

    }

    if ( Y < gotoY+50 && Y > gotoY-50 ) {
      //std::cout << "stop" << std::endl;
      MoveDown = false;
      MoveUp = false;

    }


  }*/


/* if(GotoCommand)
  {
    int num = 95;
    int den = 100;
    double saveGotoX = gotoX;
    double saveGotoY = gotoY;
     gotoX = (den-num)*(NearestEntity->X)/den+num*(gotoX)/den;
     gotoY = (den-num)*(NearestEntity->Y)/den+num*(gotoY)/den;

    //gotoX = NearestEntity->X;
    //gotoY = NearestEntity->Y;

    if ( Y < saveGotoY+50 && Y > saveGotoY-50 &&  X < saveGotoX+50 && X > saveGotoX-50) {
      //std::cout << "stop" << std::endl;
      StopMove();
      if ( X < saveGotoX+50 && X > saveGotoX-50 ) {
        //std::cout << "stop" << std::endl;
        MoveLeft = false;
        MoveRight = false;

      }

    }else{
      double normalizationFactor = 0.5 / (abs(gotoX-X)+abs(gotoY-Y));
      AccelX = normalizationFactor*(gotoX-X);
      AccelY = normalizationFactor*(gotoY-Y);

      if (gotoX < ( X -50) )  {
      //std::cout << "goto left" << std::endl;
      MoveLeft = true;
      MoveRight = false;

      }
      if(gotoX > X + 50) {
        // std::cout << "goto right" << std::endl;
        MoveRight = true;
        MoveLeft = false;

      }

      if ( X < gotoX+50 && X > gotoX-50 ) {
        //std::cout << "stop" << std::endl;
        MoveLeft = false;
        MoveRight = false;

      }
    }

    gotoX = saveGotoX;
    gotoY = saveGotoY;*/


// main process for entities, cast inside the main game loop
void CEntity::OnLoopApplyControls()
{

 if(GotoCommand)
  {

    if (gotoX < ( X -50) )  {
      //std::cout << "goto left" << std::endl;
      MoveLeft = true;
      MoveRight = false;

    }
    if(gotoX > X + 50) {
      // std::cout << "goto right" << std::endl;
      MoveRight = true;
      MoveLeft = false;

    }

    if ( X < gotoX+50 && X > gotoX-50 ) {
      //std::cout << "stop" << std::endl;
      MoveLeft = false;
      MoveRight = false;

    }

     if (gotoY < ( Y -50) )  {
      //std::cout << "goto left" << std::endl;
      MoveUp = true;
      MoveDown = false;

    }
    if(gotoY > Y + 50) {
      // std::cout << "goto right" << std::endl;
      MoveDown = true;
      MoveUp = false;

    }

    if ( Y < gotoY+50 && Y > gotoY-50 ) {
      //std::cout << "stop" << std::endl;
      MoveDown = false;
      MoveUp = false;

    }

    if ( X < gotoX+50 && X > gotoX-50 && Y < gotoY+50 && Y > gotoY-50 ) {
      //StopMove();
      GotoCommand = false;
      gotoX = X;
      gotoY = Y;


    }


  // else
  //{



  }
  //}

}

// main process for entities, cast inside the main game loop
void CEntity::OnLoopDeriveAndCapSpeed(float& dt)
{

  //We're not Moving
  if(MoveLeft == false && MoveRight == false && MoveUp == false && MoveDown == false)
  {
    StopMove(); // set acceleration to the opposite value
    // to gradually stop motion
  }

  if(MoveLeft )
  {
    AccelX = -0.5; // gradually increse speed to the left
  }
  else

    if(MoveRight)
    {
      AccelX = 0.5; // gradually increse speed to the right
    }

    if(MoveUp )
  {
    AccelY = -0.5; // gradually increse speed to the left
  }
  else

    if(MoveDown)
    {
      AccelY = 0.5; // gradually increse speed to the right
    }


  // set speed according to acceleration
  // FPS control is included to be consistent across various system perfs
  // => acceleration is constant in terms of pixel * second^-2 for diff. perfs
  SpeedX += AccelX  * dt;
  SpeedY += AccelY  * dt;

  //SpeedX += (AccelX - 0.1*SpeedX) * dt;
  //SpeedY += (AccelY - 0.1*SpeedY) * dt;

  // cap the speed
  if(SpeedX > MaxSpeedX)  SpeedX =  MaxSpeedX;
  if(SpeedX < -MaxSpeedX) SpeedX = -MaxSpeedX;
  if(SpeedY > MaxSpeedY)  SpeedY =  MaxSpeedY;
  if(SpeedY < -MaxSpeedY) SpeedY = -MaxSpeedY;



  OnAnimate(); // update entity animation

}

void CEntity::OnLoopMotionBounds(float& dt)
{

  if(SpeedX == 0 && SpeedY == 0) return;

  //NextX = X+SpeedX*dt;
  //NextY = Y+SpeedY*dt;

  NextX = X;
  NextY = Y;



}

void CEntity::OnLoopRealizeMotion(float& dt)
{


  //OnSpeculateMove(SpeedX, SpeedY, dt);

  X = NextX;
  Y = NextY;

}




// draw the entity
void CEntity::OnRender(SDL_Surface* Surf_Display)
{
  if(Surf_Entity == NULL || Surf_Display == NULL) return;



  CSurface::OnDraw(Surf_Display, Surf_Entity, // draw the entity's surface on the target surface Surf_Display
                   // camera coordinates are taken into account, so if the camera moves, the entities will be displayed accordingly
                   X - CCamera::CameraControl.GetX(),
                   Y - CCamera::CameraControl.GetY(),
                   CurrentFrameCol * Width, // X coordinate on the entity's animation tile
                   (CurrentFrameRow + Anim_Control.GetCurrentFrame()) * Height, // Y coordinate on the entity's animation tile
                   Width, Height); // to set the rectangle size on the animation tile to be displayed
}

// handles entity animation
void CEntity::OnAnimate()
{

  // give correct entity orientation
  if(MoveLeft)
  {
    CurrentFrameCol = 0;
  }
  else

    if(MoveRight)
    {
      CurrentFrameCol = 1;
    }

  Anim_Control.OnAnimate();
}

// called whenever two entities collide
bool CEntity::OnCollision(CEntity* Entity)
{
  std::cout << "Entity on collision" << std::endl;
}
// overridden in child classes

// called whenever two entities collide
bool CEntity::OnCollision(CEntity* Entity, CEntityCol* EntityCol, int nbCollisionCheck, int maxCollisionCheck)
{
  std::cout << "Entity on collision" << std::endl;
}
// overridden in child classes



void CEntity::OnSpeculateMove(float velX, float velY, float dt, bool enableJump)
{

  if(!enableJump)
    CanJump = false;
  else
    CanJump = true;

  if(velX == 0 && velY == 0) return;



  float dx = velX*dt;
  float dy = velY*dt;



  double planckX =0;
  double planckY =0;

  if(dx != 0)
  {
    if(dx >= 0)     planckX =  dt;
    else             planckX = -dt;
  }

  if(dy != 0)
  {
    if(dy >= 0)     planckY =  dt;
    else             planckY = -dt;
  }

  // collision checking loop
  // the entity moves by increments of NewX and NewY and at each iteraton a collision check is performed
  // MoveX and MoveY are decreased by increments of NewX and NewY along the way
  // once MoveX and MoveY reach 0, the entity has arrived at destination
  while(true)
  {

    // std::cout << "sheep " << id << " resolving collision on map " << std::endl;
    if(Flags & ENTITY_FLAG_GHOST)
    {
      PosValidOnMap((int)(NextX + planckX), (int)(NextY + planckY)); //We don't care about collisions, but we need to send events to other entities

      updateX(planckX);
      NextY += planckY;
    }
    else
    {

      updateX(planckX);

      bool posValid = PosValidOnMap((int)(NextX), (int)(NextY + planckY));
      //std::cout << "1" << std::endl;

      if(posValid)   // check for position validity
      {
        NextY += planckY;

      }
      else
      {
        // entity is blocked on the Y axis by an obstacle
        if(dy > 0)   // the obstacle is below the entity
        {
          CanJump = true; // meaning it can jump
        }

        SpeedY = 0;
      }
      //std::cout << "4" << std::endl;
    }

    dx += -planckX;
    dy += -planckY;

    if(planckX > 0 && dx <= 0) planckX = 0;
    if(planckX < 0 && dx >= 0) planckX = 0;

    if(planckY > 0 && dy <= 0) planckY = 0;
    if(planckY < 0 && dy >= 0) planckY = 0;

    if(dx == 0) planckX = 0;
    if(dy == 0) planckY = 0;

    if(dx == 0 && dy     == 0)     break;
    if(planckX  == 0 && planckY     == 0)     break;



  }


}









void CEntity::updateX(double& planckX)
{

  if(PosValidOnMap((int)(NextX + planckX), (int)(NextY))   )   // check for position validity
  {

    NextX += planckX;

  }
  else
  {

    SpeedX = 0;
  }

}

// deceleration
void CEntity::StopMove()
{

  MoveDown  = false;
  MoveLeft  = false;
  MoveRight  = false;
  MoveUp = false;

  if(SpeedX > 0)
  {
    AccelX = -1.5;
  }

  if(SpeedX < 0)
  {
    AccelX =  1.5;
  }

  if(SpeedX < 2.0f && SpeedX > -2.0f)
  {
    AccelX = 0;
    SpeedX = 0;
  }

   if(SpeedY > 0)
  {
    AccelY = -1.5;
  }

  if(SpeedY < 0)
  {
    AccelY =  1.5;
  }

  if(SpeedY < 2.0f && SpeedY > -2.0f)
  {
    AccelY = 0;
    SpeedY = 0;
  }
}

// deceleration relative
void CEntity::StopMove(CEntity* Support)
{

  SpeedX = Support->SpeedX;
  SpeedY = Support->SpeedY;
  AccelX = Support->AccelX;
  AccelY = Support->AccelY;

}

// follow another entity
bool CEntity::Follow(CEntity* Entity)
{
  return true;
}

bool CEntity::Stop()
{
  MoveLeft = false;
  MoveRight = false;
}



// true if the current entity collides with something else
bool CEntity::Collides(int oX, int oY, int oW, int oH, CEntityCol& ioEntityCol)
{
  int left1, left2;
  int right1, right2;
  int top1, top2;
  int bottom1, bottom2;

  int tX = (int)X + Col_X;
  int tY = (int)Y + Col_Y;

  left1 = tX;
  left2 = oX;

  right1 = left1 + Width - 1 - Col_Width;
  right2 = oX + oW - 1;

  top1 = tY;
  top2 = oY;

  bottom1 = top1 + Height - 1 - Col_Height;
  bottom2 = oY + oH - 1;

  ioEntityCol.Height = 0; // for now height and width are always >=0
  ioEntityCol.Width = 0;

  if (bottom1 < top2) return false;
  if (top1 > bottom2) return false;

  if (right1 < left2) return false;
  if (left1 > right2) return false;

  ioEntityCol.Width = std::min( abs(right1 - left2), abs(right2 - left1));
  ioEntityCol.Height = std::min( abs(bottom1 - top2), abs(bottom2 - top1));
  if(ioEntityCol.Width == 0 || ioEntityCol.Height == 0)
  {
    ioEntityCol.Width = 0;
    ioEntityCol.Height = 0;
    return false;
  }
  return true;
}

// true if the current entity collides with something else
bool CEntity::SpeculativeCollides(int oX, int oY, int oW, int oH, CEntityCol& ioEntityCol)
{
  int left1, left2;
  int right1, right2;
  int top1, top2;
  int bottom1, bottom2;

  int tX = (int)NextX + Col_X;
  int tY = (int)NextY + Col_Y;

  left1 = tX;
  left2 = oX;

  right1 = left1 + Width - 1 - Col_Width;
  right2 = oX + oW - 1;

  top1 = tY;
  top2 = oY;

  bottom1 = top1 + Height - 1 - Col_Height;
  bottom2 = oY + oH - 1;

  ioEntityCol.Height = 0; // for now height and width are always >=0
  ioEntityCol.Width = 0;

  if (bottom1 < top2) return false;
  if (top1 > bottom2) return false;

  if (right1 < left2) return false;
  if (left1 > right2) return false;

  ioEntityCol.Width = std::min( abs(right1 - left2), abs(right2 - left1));
  ioEntityCol.Height = std::min( abs(bottom1 - top2), abs(bottom2 - top1));
  if(ioEntityCol.Width == 0 || ioEntityCol.Height == 0)
  {
    ioEntityCol.Width = 0;
    ioEntityCol.Height = 0;
    return false;
  }
  return true;
}


// position validity check
bool CEntity::PosValid(int NewX, int NewY)
{
  return PosValidOnMap(NewX, NewY) && PosValidOnEntities(NewX, NewY);
}

bool CEntity::PosValidOnMap(int NewX, int NewY)
{

  // factor in collision offsets
  int StartX     = (NewX + Col_X) / TILE_SIZE;
  int StartY     = (NewY + Col_Y) / TILE_SIZE;

  int EndX    = ((NewX + Col_X) + Width - Col_Width - 1)         / TILE_SIZE;
  int EndY    = ((NewY + Col_Y) + Height - Col_Height - 1)    / TILE_SIZE;

  // determine which tiles our entity is over
  for(int iY = StartY; iY <= EndY; iY++)
  {
    for(int iX = StartX; iX <= EndX; iX++)
    {
      CTile* Tile = CArea::AreaControl.GetTile(iX * TILE_SIZE, iY * TILE_SIZE);

      if(PosValidTile(Tile) == false)   // check collision with the map
      {
        return false;
      }
    }
  }
  return true;
}


/*bool CEntity::PosValidOnMap2(int NewX, int NewY)
{

    // factor in collision offsets
    int StartX     = (NewX + Col_X) / TILE_SIZE;
    int StartY     = (NewY + Col_Y) / TILE_SIZE;

    int EndX    = ((NewX + Col_X) + Width - Col_Width - 1)         / TILE_SIZE;
    int EndY    = ((NewY + Col_Y) + Height - Col_Height - 1)    / TILE_SIZE;

    // determine which tiles our entity is over

    std::vector<CTile*> overlappingTiles;
    bool posValidOnTiles = true;
    for(int iY = StartY; iY <= EndY; iY++)
    {
        for(int iX = StartX; iX <= EndX; iX++)
        {
            CTile* Tile = CArea::AreaControl.GetTile(iX * TILE_SIZE, iY * TILE_SIZE);
            overlappingTiles.push_back(Tile);
            if(PosValidTile(Tile) == false)   // check collision with the map
            {
                posValidOnTiles = false;
            }
        }
    }
    if(posValidOnTiles)
        return true;

    // position is not valid on tiles, we need to create a collision between the map and the entity

}*/


bool CEntity::PosValidOnEntities(int NewX, int NewY)
{
  bool result = true;
  if(Flags & ENTITY_FLAG_MAPONLY)
  {
  }
  else
  {

    double distance = 99999999999;

    for(int i = 0; i < EntityList.size(); i++)
    {

      if(this != EntityList[i] && abs(EntityList[i]->X-X)+abs(EntityList[i]->Y-Y) < distance) {
        distance = abs(EntityList[i]->X-X)+abs(EntityList[i]->Y-Y);
        //std::cout << distance << std::endl;
        NearestEntity = EntityList[i];
      }

      if(PosValidEntity(EntityList[i], NewX, NewY) == false)   // check collision with another entity
      {
        result = false;
      }
    }
  }
  return result;
}


// position validity check against the map
bool CEntity::PosValidTile(CTile* Tile)
{
  if(Tile == NULL)
  {
    return true;
  }

  if(Tile->TypeID == TILE_TYPE_BLOCK)
  {
    return false;
  }

  return true;
}

// position validity check against other entity
// adds a new inter entity collision in the entity collision stack
bool CEntity::PosValidEntity(CEntity* Entity, int NewX, int NewY)
{


  bool result = true;

  if(this != Entity && Entity != NULL && Entity->Dead == false &&
      Entity->Flags ^ ENTITY_FLAG_MAPONLY)
  {

    CEntityCol EntityCol;
    result = !Entity->SpeculativeCollides(NewX + Col_X, NewY + Col_Y, Width - Col_Width, Height - Col_Height, EntityCol);


    EntityCol.EntityA = this;

    EntityCol.EntityB = Entity;


    if(!result)
      CEntityCol::EntityColList.insert(std::make_pair(std::make_pair(entityId, Entity->entityId), EntityCol));


  }

  return result;
}

void CEntity::OnCleanup()
{
  if(Surf_Entity)
  {
    SDL_FreeSurface(Surf_Entity);
  }

  Surf_Entity = NULL;
}









