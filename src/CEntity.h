#ifndef _CENTITY_H_
#define _CENTITY_H_

#include <vector>
#include <map>
#include <set>

#include "CArea.h"
#include "CAnimation.h"
#include "CCamera.h"

#include "CSurface.h"


#include "Vectorial.h"

class CEntityCol;

class CEntity {

  // Enums used in this class
  typedef enum {
    ENTITY_TYPE_GENERIC = 0,
    ENTITY_TYPE_PLAYER
  } EN_EntityType;

  typedef enum {
    ENTITY_FLAG_NONE     = 0,
    ENTITY_FLAG_GRAVITY = 0x00000001,
    ENTITY_FLAG_GHOST = 0x00000002,
    ENTITY_FLAG_MAPONLY = 0x00000004
  } EN_EntityBehavior;

  // Friend classes
  friend class CFollower;
  friend class Effect;
  friend class Switch;
  friend class Bomb;

  // Public members
  public:

  void generateRandom(Rectangle& boundaries);

  static void dist(CEntity& entityA, CEntity& entityB, PointDouble& oResult);

  static void dist(CEntity& entity, PointInt& point, PointDouble& oResult);
  static void dist(CEntity& entity, PointDouble& point, PointDouble& oResult);
  static void signedDist(CEntity& entity, PointDouble& point, PointDouble& oResult);

  // keeps track of all entities addresses
  static std::vector<CEntity*> EntityList;

  // construct new entity
  CEntity();

  // destroy entity
  virtual ~CEntity();

  // load a new entity in memory
  virtual bool OnLoad(char* iFile, Rectangle& iMask, int iMaxFrames);

  // apply controls from the player -> should be moved to child class
  void OnLoopApplyControls();

  // limit the speed of entities -> might need to be overridden in child class
  void OnLoopDeriveAndCapSpeed(double& dt);

  // speculative collision : initiate next position
  void OnLoopMotionBounds();

  // speculative collision : update position to next position
  void OnLoopRealizeMotion();

  // speculative collision : calculate next position regarding the map
  void OnMove(PointDouble& iVelocity, double& dt);

  // render the entity on the display surface
  virtual void OnRender(SDL_Surface* ioSdlDisplaySurface);

  // operations to be done at clean up time : free memory
  virtual void OnCleanup();

  // animate the sprite of the entity
  virtual void OnAnimate();

  // stop the motion
  void StopMove();

  // check for a collision with input rectangle, create an collision object if it is the case
  bool Collides(Rectangle& iOtherRectangle, CEntityCol& ioEntityCol);

  // stop the controls
  bool Stop();

  // check if the entity as a valid position regarding other entities
  bool PosValidOnEntities();

  // get the entity's top left corner's position
  PointDouble& getPosition() { return _position; }

  void setPosition(PointDouble& iNewPosition, bool next=false) {
    _position = iNewPosition;
    _mask.getCorner().set(_position.getX(), _position.getY());
    _center = _mask.getCenter();
    

    if(next) {
      _nextPosition = iNewPosition; }
  }

  // get the entity's rectangle center
  PointInt& getCenter() { return _center; }

  // get the entity's speed
  PointDouble& getSpeed() { return _speed; }

  // get the entity's maximum speed
  PointDouble& getMaxSpeed() { return _maxSpeed; }

  // get the entity's acceleration
  PointDouble& getAccel() { return _accel; }

  // get the entity's speculative position
  PointDouble& getNextPosition() { return _nextPosition; }

  // get the position towards which the entity is aiming
  PointDouble& getTargetPosition() { return _targetPosition; }

  // to know if the entity is currently aiming somewhere
  bool isTargettingPosition() { return _isTargettingPosition; }

  void isTargettingPosition(bool isIt) { _isTargettingPosition = isIt; }

  // the base rectangle of this entity (i.e. its sprite)
  Rectangle& getMask() { return _mask; }

  // the collision mask of this entity
  Rectangle& getCollisionMask() { return _collisionMask; }
  void updateCollisionMask();
  static int CurrentEntityId;

  void setEntityId(int iId) { _entityId = iId;}
  int& getEntityId() { return _entityId; }

  bool& removeAtNextLoop() { return _removeAtNextLoop; }


  // locks
  int _lockLeft;
  int _lockRight;
  int _lockDown;
  int _lockUp;

  protected:
  bool _removeAtNextLoop;

  bool PosValidOnMap(PointDouble& NewPosition);
  bool PosValidTile(CTile* Tile);
  bool PosValidEntity(CEntity* Entity);


  PointDouble _position;

  PointInt _center;

  PointDouble _speed;

  PointDouble _maxSpeed;

  PointDouble _accel;

  PointDouble _nextPosition;

  PointDouble _targetPosition;

  bool _isTargettingPosition;

  Rectangle _mask;

  Rectangle _collisionMask;
  Rectangle _collisionOffset;

  // controls
  bool _moveLeft;
  bool _moveRight;
  bool _moveUp;
  bool _moveDown;

  // entity state
  int _entityId;
  EN_EntityType _type;
  bool _dead;
  EN_EntityBehavior _behavior; // ghost or dead ...

  // tile on which the entity reside
  int _tileId;
  int _tileType;
  int _nextTileId;
  int _nextTileType;

  // animation
  SDL_Surface* _sdlSurface;
  CAnimation _animControl;
  int _currentFrameCol;
  int _currentFrameRow;


};


class CEntityCol {
  public:
    static std::map<std::pair<int, int>, CEntityCol>    EntityColList;

    // rectangle of the intersection of bounding boxes for this collision
    Rectangle _rectangle;

  public:
    CEntity* EntityA;

    CEntity* EntityB;

    void updateContactSize();

  public:
    CEntityCol();
};



#endif
