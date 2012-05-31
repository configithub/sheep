#include "CEntity.h"
#include <iostream>
#include <math.h>
#include <algorithm>


std::vector<CEntity*>     CEntity::EntityList;
int CEntity::CurrentEntityId = 0;



CEntity::CEntity()
{
  _sdlSurface = NULL;

  // entity coordinates and size
  _position.set(100,100);


  _isTargettingPosition = false;

  _nextPosition.set(100, 100);

  _mask.set(_position.getX(), _position.getY(), 0, 0);

  // animate left or right?
  _moveLeft  = false;
  _moveRight = false;
  _moveUp  = false;
  _moveDown = false;

  // entity type
  _type = ENTITY_TYPE_GENERIC;

  _dead = false;
  _behavior = ENTITY_FLAG_GRAVITY;

  // movement parameters

  // speed, pixel * second^-1
  _speed.set(0,0);

  // acceleration, pixel * second^-2
  _accel.set(0,0);

  // max speed, pixel * second^-1
  _maxSpeed.set(50, 50);

  // animation tile settings
  _currentFrameCol = 0;
  _currentFrameRow = 0;

  // collision mask of the entity
  // values are offsets (i.e. margins)
  _collisionMask.set( 0,0, 0, 0);
  _collisionOffset.set( 0, -5, 0, -2);

  // to identify the entity for debugging
  _entityId=CEntity::CurrentEntityId;
  std::cout << "creating entity " << _entityId << std::endl;
  
  ++CEntity::CurrentEntityId;

}



CEntity::~CEntity()
{
}

bool CEntity::OnLoad(char* iFile, Rectangle& iRectangle, int iMaxFrames) {
  if((_sdlSurface = CSurface::OnLoad(iFile)) == NULL) {
    return false;
  }

  this->_mask = iRectangle;
  _animControl.MaxFrames = iMaxFrames;
  return true;
}


// 1. factor in controls applied by the player
void CEntity::OnLoopApplyControls() {
 if(_isTargettingPosition) {

    PointDouble distanceToTarget = _targetPosition - _position;


    _moveLeft = false; _moveRight = false; _moveUp = false; _moveDown = false;
    _moveLeft = distanceToTarget.getX() < -20;
    _moveRight = distanceToTarget.getX() > 20;
    _moveUp = distanceToTarget.getY() < -20;
    _moveDown = distanceToTarget.getY() > 20;

    if ( !_moveLeft && !_moveRight && !_moveUp && !_moveDown ) {
      //StopMove();
      _isTargettingPosition = false;
      _targetPosition = _position;
    }
  }
}


// 2. calculate natural accelerations and speeds
void CEntity::OnLoopDeriveAndCapSpeed(double& dt) {

  //We're not Moving
  if ( !_moveLeft && !_moveRight && !_moveUp && !_moveDown ) { StopMove(); }

  if(_moveLeft ) { _accel.setX(-0.5); }
  else if(_moveRight) { _accel.setX(0.5); }
  if(_moveUp ) { _accel.setY(-0.5); }
  else if(_moveDown) { _accel.setY(0.5); }

  // set speed according to acceleration
  // FPS control is included to be consistent across various system perfs
  // => acceleration is constant in terms of pixel * second^-2 for diff. perfs

  _speed += (_accel - _speed*(double)0.1) * dt;

  // cap the speed
  _speed.cap(_maxSpeed);

  OnAnimate(); // update entity animation
}

// 3. initiate nextX and nextY
void CEntity::OnLoopMotionBounds() {
  if(_speed.modulus() == 0) return;
  _nextPosition.set(_position.getX(), _position.getY());
}



void CEntity::OnLoopRealizeMotion() {
  _position.set((_nextPosition.getX()), (_nextPosition.getY()) );
  _tileId = _nextTileId;
  _tileType = _nextTileType;
}




// draw the entity
void CEntity::OnRender(SDL_Surface* Surf_Display) {
  if(_sdlSurface == NULL || Surf_Display == NULL) return;

  CSurface::OnDraw(Surf_Display, _sdlSurface, // draw the entity's surface on the target surface Surf_Display
                   // camera coordinates are taken into account, so if the camera moves, the entities will be displayed accordingly
                   _position.getX() - CCamera::CameraControl.GetX(),
                   _position.getY() - CCamera::CameraControl.GetY(),
                   _currentFrameCol * _mask.getWidth(), // X coordinate on the entity's animation tile
                   (_currentFrameRow + _animControl.GetCurrentFrame()) * _mask.getHeight(), // Y coordinate on the entity's animation tile
                   _mask.getWidth(), _mask.getHeight()); // to set the rectangle size on the animation tile to be displayed
}

// handles entity animation
void CEntity::OnAnimate() {
  // give correct entity orientation
  _animControl.OnAnimate();
}


void CEntity::OnMove(Point<double>& vel, double& dt) {
  if(vel.modulus() == 0) return;

  Point<double> dl = vel*dt;
  Point<double> planck(sgn<double>(dl.getX())*dt, sgn<double>(dl.getY())*dt);


  // collision checking loop
  // the entity moves by increments of NewX and NewY and at each iteraton a collision check is performed
  // MoveX and MoveY are decreased by increments of NewX and NewY along the way
  // once MoveX and MoveY reach 0, the entity has arrived at destination
  while(true) {

    Point<double> testPosition((_nextPosition.getX()+planck.getX()), (_nextPosition.getY()+planck.getY()));
    if(_behavior & ENTITY_FLAG_GHOST) {
      PosValidOnMap(testPosition);
      _nextPosition = testPosition;
    } else {

      Point<double> testPositionX((_nextPosition.getX()+planck.getX()), (_nextPosition.getY()));
      if( PosValidOnMap(testPositionX) ) {
        _nextPosition = testPositionX;

      } else { _speed.setX(0); }

      Point<double> testPositionY((_nextPosition.getX()), (_nextPosition.getY()+planck.getY()));
      if( PosValidOnMap(testPositionY) ) {
        _nextPosition = testPositionY;

      } else { _speed.setY(0); }

    }
    dl -= planck;

    if( (planck.getX() > 0 && dl.getX() <= 0) || (planck.getX() < 0 && dl.getX() >= 0) ) { planck.setX(0); }
    if( (planck.getY() > 0 && dl.getY() <= 0) || (planck.getY() < 0 && dl.getY() >= 0) ) { planck.setY(0); }
    if(dl.getX() == 0) { planck.setX(0);}
    if(dl.getY() == 0) { planck.setY(0);}
    if(dl.getX() == 0 && dl.getY() == 0 ) { break; }
    if(planck.getX() == 0  && planck.getY() == 0 ) { break; }

  }

}


// deceleration
void CEntity::StopMove() {
  Stop();
  if(_speed.getX() > 0) { _accel.setX(-1.5); }
  if(_speed.getX() < 0) { _accel.setX(1.5); }
  if(_speed.getX() < 2.0f && _speed.getX() > -2.0f) {
    _accel.setX(0);
    _speed.setX(0);
  }

  if(_speed.getY() > 0) { _accel.setY(-1.5); }
  if(_speed.getY() < 0) { _accel.setY(1.5); }
  if(_speed.getY() < 2.0f && _speed.getY() > -2.0f) {
    _accel.setY(0);
    _speed.setY(0);
  }
}

bool CEntity::Stop() {
  _moveDown = false; _moveLeft = false;
  _moveRight = false; _moveUp = false;
  _isTargettingPosition = false;
}




bool CEntity::PosValidOnMap(PointDouble& iNewPosition) {

  // factor in collision offsets
  int StartX     = (iNewPosition.getX() + _collisionOffset.getCorner().getX()) / TILE_SIZE;
  int StartY     = (iNewPosition.getY() + _collisionOffset.getCorner().getY()) / TILE_SIZE;

  int EndX    = ((iNewPosition.getX() + _collisionOffset.getCorner().getX()) + _mask.getWidth() + _collisionOffset.getWidth() - 1)         / TILE_SIZE;
  int EndY    = ((iNewPosition.getY() + _collisionOffset.getCorner().getY()) + _mask.getHeight() + _collisionOffset.getHeight() - 1)    / TILE_SIZE;

  // determine which tiles our entity is over
  for(int iY = StartY; iY <= EndY; iY++) {
    for(int iX = StartX; iX <= EndX; iX++) {
      CTile* Tile = CArea::AreaControl.GetTile(iX * TILE_SIZE, iY * TILE_SIZE);
      _nextTileId = Tile->TileID;
      _nextTileType = Tile->TypeID;
      // check collision with the map
      if(PosValidTile(Tile) == false)   { return false; }
    }
  }
  return true;
}

// position validity check against the map
bool CEntity::PosValidTile(CTile* Tile) {
  if(Tile == NULL) { return true; }
  if(Tile->TypeID == TILE_TYPE_BLOCK) { return false; }
  return true;
}

// true if the current entity collides with something else
bool CEntity::Collides(Rectangle& iOtherObject, CEntityCol& ioEntityCol) {
  updateCollisionMask();
  _collisionMask.intersect(iOtherObject, ioEntityCol._rectangle);
  if(ioEntityCol._rectangle.isVoid()) { return false; }
  return true;
}

void CEntity::updateCollisionMask() {
  //_collisionMask.set( _nextPosition.getX() , _nextPosition.getY() ,
  //       _mask.getWidth() , _mask.getHeight() );

  _collisionMask.set( _nextPosition.getX() + _collisionOffset.getCorner().getX(), _nextPosition.getY() + _collisionOffset.getCorner().getY(),
      _mask.getWidth() + _collisionOffset.getWidth(), _mask.getHeight() + _collisionOffset.getHeight() );
}

// detect all collisions
bool CEntity::PosValidOnEntities() {
  bool result = true;
  if(_behavior & ENTITY_FLAG_MAPONLY || _behavior & ENTITY_FLAG_GHOST) {
  } else {
    updateCollisionMask();
    for(int i = 0; i < EntityList.size(); i++) {
      // check collision with another entity
      if(!PosValidEntity(EntityList[i])) {
        result = false;
      }
    }
  }
  return result;
}

// position validity check against other entity
// adds a new inter entity collision in the entity collision stack
bool CEntity::PosValidEntity(CEntity* iEntity) {
  bool result = true;
  if(this != iEntity && iEntity != NULL && iEntity->_dead == false &&
      iEntity->_behavior ^ ENTITY_FLAG_MAPONLY) {

    CEntityCol EntityCol;

    result = !iEntity->Collides(_collisionMask, EntityCol);
    EntityCol.EntityA = this;
    EntityCol.EntityB = iEntity;
    if(!result) {
      int idA; int idB;
      if(_entityId > iEntity->_entityId) { idA = iEntity->_entityId; idB = _entityId; } else{ idB = iEntity->_entityId; idA = _entityId; }
      CEntityCol::EntityColList.insert(std::make_pair(std::make_pair(idA, idB), EntityCol));
    }
  }
  return result;
}

void CEntity::OnCleanup() {
  if(_sdlSurface) { SDL_FreeSurface(_sdlSurface); }
  _sdlSurface = NULL;
}

void CEntity::dist(CEntity& entityA, CEntity& entityB, PointDouble& oResult) {
  distance(entityA.getPosition(), entityB.getPosition(), oResult);
}

void CEntity::dist(CEntity& entity, PointInt& point, PointDouble& oResult) {
  distance(entity.getCenter(), point, oResult);
}

