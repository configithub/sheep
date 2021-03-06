#include "Entity.h"
#include "CApp.h"
#include <iostream>
#include <math.h>
#include <algorithm>


std::vector<Entity*> Entity::EntityList;
std::vector<Entity*> Entity::NextEntityList;
std::vector<Entity*> Entity::EntityListToRemove;
int Entity::CurrentEntityId = 0;

Entity::Entity()
{
  _glTexture = 0;

  _isTranslating = false;

  // entity coordinates and size
  _position.set(100,100);

  // selection
  _selection = 0;


  _isTargettingPosition = false;

  _nextPosition.set(100, 100);

  _mask.set(_position.getX(), _position.getY(), 0, 0);
  _drawRectangle.set(_position.getX(), _position.getY(), 0, 0);

  // animate left or right?
  _moveLeft  = false;
  _moveRight = false;
  _moveUp  = false;
  _moveDown = false;
  _moveDirection = PointDouble(0, 0);

  // entity type
  // _type = ENTITY_TYPE_GENERIC; // TODO: move this kind of stuff in behavior

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

  _removeAtNextLoop = false;

  // to identify the entity for debugging
  _entityId=Entity::CurrentEntityId;

  ++Entity::CurrentEntityId;

}

void Entity::generateRandom(Rectangle& iBoundaries, EN_EntityType iType, CApp* iParent) {
  int x,y;
  x = (int)(rand() % (iBoundaries.getWidth() ) + iBoundaries.getCorner().getX());
  y = (int)(rand() % (iBoundaries.getHeight() ) + iBoundaries.getCorner().getY());
  this->setParent(iParent);
  PointDouble randPos(x,y);
  if(!this->PosValidOnMap(randPos)) { this->generateRandom(iBoundaries, iType, iParent); }else{
    generateAtPos(randPos, iType, iParent);
  }

}

void Entity::generateAtPos(PointDouble& iPosition, EN_EntityType iType, CApp* iParent) {
  _position = iPosition;
  _nextPosition = _position;

  setParent(iParent);

  _glTexture = Texture::Sprites[iType];
  Rectangle mask;
  Rectangle drawRectangle;

  _type = iType;
  b = NULL;

  switch(iType) {
    case(SHEEP):
      mask = Rectangle(0,0,32,32);
      drawRectangle = Rectangle(0,0,32,32);
      OnLoad(mask, drawRectangle, 4);
      _isTranslating = true;
      b = &_parent->SheepPool[_entityId];
    break;
    case(BOMB):
      mask = Rectangle(0,0,32,32);
      drawRectangle = Rectangle(0,0,32,32);
      OnLoad(mask, drawRectangle, 4);
      b = &_parent->BombPool[_entityId];
    break;
    case(SWITCH):
      mask = Rectangle(0,0,32,32);
      drawRectangle = Rectangle(0,0,32,32);
      OnLoad(mask, drawRectangle, 4);
      b = &_parent->SwitchPool[_entityId];
    break;
    case(EFFECT):
      mask = Rectangle(0,0,32,32);
      drawRectangle = Rectangle(0,0,32,32);
      OnLoad(mask, drawRectangle, 4);
      b = &_parent->EffectPool[_entityId];
    break;
    case(SAW):
      mask = Rectangle(0,0,32,32);
      drawRectangle = Rectangle(0,0,32,32);
      OnLoad(mask, drawRectangle, 4);
      b = &_parent->SawPool[_entityId];
    break;
    case(DOOR):
      mask = Rectangle(0,0,32,64);
      drawRectangle = Rectangle(0,0,32,64);
      OnLoad(mask, drawRectangle, 4);
      b = &_parent->DoorPool[_entityId];
    break;
  }

  if(b != NULL) {
    b->e = this; b->OnInit();
    Entity::EntityList.push_back(this);
  }else{
    std::cout << "entity creation failed, null behavior" << std::endl;
  }

}



Entity::~Entity() { }

void Entity::OnLoop() {

  // Type independant
  if(!_removeAtNextLoop) {
    Entity::NextEntityList.push_back(this);
  }else{
    Entity::EntityListToRemove.push_back(this);
  }
  // Type dependant
  b->OnLoop();
}


bool Entity::OnLoad(Rectangle& iMask, Rectangle& iDrawRectangle, int iMaxFrames) {
  this->_mask = iMask;
  this->_drawRectangle = iDrawRectangle;
  _animControl.MaxFrames = iMaxFrames;
  return true;
}


// 1. factor in controls applied by the player
void Entity::OnLoopApplyControls(double& dt) {
  PointDouble distanceToTarget(0, 0);
  if(_isTargettingPosition) {

    Entity::signedDist(*this, _targetPosition, distanceToTarget);

    _moveLeft = false; _moveRight = false; _moveUp = false; _moveDown = false;
    _moveLeft = distanceToTarget.getX() < -5;
    _moveRight = distanceToTarget.getX() > 5;
    _moveUp = distanceToTarget.getY() < -5;
    _moveDown = distanceToTarget.getY() > 5;

    if ( !_moveLeft && !_moveRight && !_moveUp && !_moveDown ) {
      _isTargettingPosition = false;
      _targetPosition = _position;
    }
  }

  //We're not Moving
  if ( !_moveLeft && !_moveRight && !_moveUp && !_moveDown ) { StopMove(dt); }
  else { 
    _moveDirection = distanceToTarget.normalize(1);
    _accel.setX( _moveDirection.getX() );
    _accel.setY( _moveDirection.getY() );
  }

  // set speed according to acceleration
  // FPS control is included to be consistent across various system perfs
  // => acceleration is constant in terms of pixel * second^-2 for diff. perfs
  _speed += (_accel - _speed*(double)0.1) * dt;
  //_speed += _accel * dt;

  // cap the speed
  _speed.cap(_maxSpeed);

}

// 3. initiate nextX and nextY
void Entity::OnLoopMotionBounds() {
  if(_speed.modulus() == 0) return;
  _nextPosition.set(_position.getX(), _position.getY());
}



void Entity::OnLoopRealizeMotion() {
  //_position.set((_nextPosition.getX()), (_nextPosition.getY()) );

  setPosition(_nextPosition);

  _tileId = _nextTileId;
  _tileType = _nextTileType;
}


void Entity::setPosition(PointDouble& iNewPosition, bool next) {
  _position = iNewPosition;
  _mask.getCorner().set(_position.getX(), _position.getY());
  _center = _mask.getCenter();

  if(next) { _nextPosition = iNewPosition; }

}

// draw the entity
void Entity::OnRender() {
  OnAnimate(); // update entity animation
  if( _glTexture == 0 ) return;
  b->OnRender();
  Texture::draw(_glTexture, // draw the entity's surface on the target surface Surf_Display
      // camera coordinates are taken into account, so if the camera moves, the entities will be displayed accordingly
      _position.getX() - Camera::CameraControl.GetX(),
      _position.getY() - Camera::CameraControl.GetY(),
      _currentFrameCol * _drawRectangle.getWidth(), // X coordinate on the entity's animation tile
      (_currentFrameRow + _animControl.GetCurrentFrame()) * _drawRectangle.getHeight(), // Y coordinate on the entity's animation tile
      _drawRectangle.getWidth(), _drawRectangle.getHeight()); // to set the rectangle size on the animation tile to be displayed
}

// handles entity animation
void Entity::OnAnimate() {
  // type dependant animation
  b->OnAnimate();
  // type independant stuff
  _animControl.OnAnimate();
}


void Entity::OnMove(Point<double>& vel, double& dt) {
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

      } else { 
        _speed.setX(0);
      }

      Point<double> testPositionY((_nextPosition.getX()), (_nextPosition.getY()+planck.getY()));
      if( PosValidOnMap(testPositionY) ) {
        _nextPosition = testPositionY;

      } else {
        _speed.setY(0);
      }

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
// todo : replace 0 with resting momentum for speed
void Entity::StopMove(double dt) {
  int resting_speed = 0;
  _moveDown = false; _moveLeft = false;
  _moveRight = false; _moveUp = false;
  _isTargettingPosition = false;
  if(_speed.getX() > resting_speed) { _accel.setX(-1.5); }
  if(_speed.getX() < resting_speed) { _accel.setX(1.5); }
  if(_speed.getX() - resting_speed < 2.0f && _speed.getX() - resting_speed > -2.0f) {
    _accel.setX(0);
    _speed.setX(resting_speed);
  }

  if(_speed.getY() > 0) { _accel.setY(-1.5); }
  if(_speed.getY() < 0) { _accel.setY(1.5); }
  if(_speed.getY() < 2.0f && _speed.getY() > -2.0f) {
    _accel.setY(0);
    _speed.setY(0);
  }
  b->OnStop();
}





bool Entity::PosValidOnMap(PointDouble& iNewPosition) {

  // factor in collision offsets
  int StartX     = (iNewPosition.getX() + _collisionOffset.getCorner().getX()) / TILE_SIZE;
  int StartY     = (iNewPosition.getY() + _collisionOffset.getCorner().getY()) / TILE_SIZE;

  int EndX    = ((iNewPosition.getX() + _collisionOffset.getCorner().getX()) + _mask.getWidth() + _collisionOffset.getWidth() - 1)         / TILE_SIZE;
  int EndY    = ((iNewPosition.getY() + _collisionOffset.getCorner().getY()) + _mask.getHeight() + _collisionOffset.getHeight() - 1)    / TILE_SIZE;

  // determine which tiles our entity is over
  for(int iY = StartY; iY <= EndY; iY++) {
    for(int iX = StartX; iX <= EndX; iX++) {
      Tile* Tile = Area::AreaControl.GetTile(iX * TILE_SIZE, iY * TILE_SIZE);
      _nextTileId = Tile->TileID;
      _nextTileType = Tile->TypeID;
      // check collision with the map
      if(PosValidTile(Tile) == false)   { return false; }
      // Door step
      switch(Tile->TypeID) {
        case DOOR_STEP_RIGHT:
          if(this->_moveRight) {
            this->isTargettingPosition(true);
            this->getTargetPosition().set(_position.getX()+DOOR_MOVE_DISTANCE,_position.getY());
          }
          break;
        case DOOR_STEP_LEFT:
          if(this->_moveLeft) {
            this->isTargettingPosition(true);
            this->getTargetPosition().set(_position.getX()-DOOR_MOVE_DISTANCE,_position.getY());
          }
          break;
        case DOOR_STEP_UP:
          if(this->_moveUp) {
            this->isTargettingPosition(true);
            this->getTargetPosition().set(_position.getX(), _position.getY()-DOOR_MOVE_DISTANCE);
          }
          break;
        case DOOR_STEP_DOWN:
          if(this->_moveDown) {
            this->isTargettingPosition(true);
            this->getTargetPosition().set(_position.getX(), _position.getY()+DOOR_MOVE_DISTANCE);
          }
          break;
      }


    }
  }
  return true;
}

// position validity check against the map
bool Entity::PosValidTile(Tile* Tile) {
  if(Tile == NULL) { return true; }
  if(Tile->TypeID == TILE_TYPE_BLOCK) { return false; }
  return true;
}

// true if the current entity collides with something else
bool Entity::Collides(Rectangle& iOtherObject, EntityCol& ioEntityCol) {
  updateCollisionMask();
  _collisionMask.intersect(iOtherObject, ioEntityCol._rectangle);
  if(ioEntityCol._rectangle.isVoid()) { return false; }
  return true;
}

void Entity::updateCollisionMask() {
  //_collisionMask.set( _nextPosition.getX() , _nextPosition.getY() ,
  //       _mask.getWidth() , _mask.getHeight() );

  _collisionMask.set( _nextPosition.getX() + _collisionOffset.getCorner().getX(), _nextPosition.getY() + _collisionOffset.getCorner().getY(),
      _mask.getWidth() + _collisionOffset.getWidth(), _mask.getHeight() + _collisionOffset.getHeight() );
}

// detect all collisions
bool Entity::PosValidOnEntities() {

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
bool Entity::PosValidEntity(Entity* iEntity) {

  bool result = true;
  if(this != iEntity && iEntity != NULL && iEntity->_dead == false &&
      iEntity->_behavior ^ ENTITY_FLAG_MAPONLY) {

    EntityCol EntityCol;

    result = !iEntity->Collides(_collisionMask, EntityCol);
    EntityCol.EntityA = this;
    EntityCol.EntityB = iEntity;
    if(!result) {

      int idA; int idB;
      if(_entityId > iEntity->_entityId) { idA = iEntity->_entityId; idB = _entityId; } else{ idB = iEntity->_entityId; idA = _entityId; }
      EntityCol::EntityColList.insert(std::make_pair(std::make_pair(idA, idB), EntityCol));
    }
  }

  return result;
}

void Entity::adjustForTranslation(double& moveX, double& moveY, double& dt) {
  PointDouble translateVect = PointDouble(moveX, 0);
  OnMove(translateVect, dt);
}



void Entity::OnCleanup() {
  b->OnCleanup();
  // if(_glTexture!=NULL) { SDL_FreeSurface(_glTexture); }
  // _glTexture = NULL;
  CApp::EntityPool.erase(_entityId);
}

void Entity::dist(Entity& entityA, Entity& entityB, PointDouble& oResult) {
  distance(entityA.getPosition(), entityB.getPosition(), oResult);
}

void Entity::dist(Entity& entity, PointInt& point, PointDouble& oResult) {
  distance(entity.getCenter(), point, oResult);
}

void Entity::dist(Entity& entity, PointDouble& point, PointDouble& oResult) {
  //oResult.set( abs(entity.getCenter().getX() - point.getX()) , abs(entity.getCenter().getY() - point.getY()) );
  oResult.set( abs(entity.getPosition().getX() - point.getX()) , abs(entity.getPosition().getY() - point.getY()) );
}

void Entity::signedDist(Entity& entity, PointDouble& point, PointDouble& oResult) {
  //oResult.set( abs(entity.getCenter().getX() - point.getX()) , abs(entity.getCenter().getY() - point.getY()) );
  oResult.set( ( - entity.getCenter().getX() + point.getX()) , ( - entity.getCenter().getY() + point.getY()) );
}

