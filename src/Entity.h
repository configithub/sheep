#ifndef _CENTITY_H_
#define _CENTITY_H_

#include <vector>
#include <map>
#include <set>

#include "Area.h"
#include "Animation.h"
#include "Camera.h"
#include "Texture.h"
#include "Vectorial.h"
#include "Behavior.h"

class EntityCol;
class CApp;

// Enums used in this class
typedef enum {
  SHEEP = 0,
  BOMB,
  SWITCH,
  EFFECT,
  SAW,
  DOOR,
  NUM_TYPE
} EN_EntityType;

class Entity {


  typedef enum {
    ENTITY_FLAG_NONE     = 0,
    ENTITY_FLAG_GRAVITY = 0x00000001,
    ENTITY_FLAG_GHOST = 0x00000002,
    ENTITY_FLAG_MAPONLY = 0x00000004
  } EN_EntityBehavior;

  // Friend classes
  friend class Follower;
  friend class Effect;
  friend class Switch;
  friend class Bomb;
  friend class Saw;
  friend class Door;

  // Public members
  public:

  EN_EntityType getType() { return _type; }

  void generateRandom(Rectangle& iBoundaries, EN_EntityType iType, CApp* iParent);

  void generateAtPos(PointDouble& iPosition, EN_EntityType iType, CApp* iParent);

  static void dist(Entity& entityA, Entity& entityB, PointDouble& oResult);

  static void dist(Entity& entity, PointInt& point, PointDouble& oResult);
  static void dist(Entity& entity, PointDouble& point, PointDouble& oResult);
  static void signedDist(Entity& entity, PointDouble& point, PointDouble& oResult);

  // keeps track of all current entities addresses
  static std::vector<Entity*> EntityList;

  // keeps track of all surviving entities addresses
  static std::vector<Entity*> NextEntityList;

  // keeps track of all disapearing entities addresses
  static std::vector<Entity*> EntityListToRemove;

  // construct new entity
  Entity();

  // destroy entity
  virtual ~Entity();

  void setParent(CApp* aParent) { _parent = aParent; }

  // load a new entity in memory
  bool OnLoad(Rectangle& iMask, Rectangle& iDrawRectangle, int iMaxFrames) ;

  // decide whether this entity will be part of the next loop or not
  virtual void OnLoop() ;

  // apply controls from the player -> should be moved to child class
  void OnLoopApplyControls(double& dt);

  // speculative collision : initiate next position
  void OnLoopMotionBounds();

  // speculative collision : update position to next position
  void OnLoopRealizeMotion();

  // speculative collision : calculate next position regarding the map
  void OnMove(PointDouble& iVelocity, double& dt);

  // animate the sprite of the entity
  virtual void OnAnimate();

  // render the entity on the display surface
  virtual void OnRender();

  // operations to be done at clean up time : free memory
  virtual void OnCleanup();

  // check for a collision with input rectangle, create an collision object if it is the case
  bool Collides(Rectangle& iOtherRectangle, EntityCol& ioEntityCol);

  void StopMove(double dt = 1.);

  // check if the entity as a valid position regarding other entities
  bool PosValidOnEntities();

  // get the entity's top left corner's position
  PointDouble& getPosition() { return _position; }

  int getAbsX() { return (_position.getX() - Camera::CameraControl.GetX()); } 
  int getAbsY() { return (_position.getY() - Camera::CameraControl.GetY()); } 

  // set the entity's top left corner's position
  void setPosition(PointDouble& iNewPosition, bool next=false) ;

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

  // the collision mask of this entity (i.e. its sprite)
  Rectangle& getMask() { return _mask; }

  Rectangle& getDrawRectangle() { return _drawRectangle; }

  // the collision mask of this entity
  Rectangle& getCollisionMask() { return _collisionMask; }
  void updateCollisionMask();
  static int CurrentEntityId;

  void setEntityId(int iId) { _entityId = iId;}
  int getEntityId() { return _entityId; }

  bool& removeAtNextLoop() { return _removeAtNextLoop; }

  void adjustForTranslation(double& moveX, double& moveY, double& dt);

  CApp* _parent;


  // selection
  int _selection;

  protected:
  bool _removeAtNextLoop;

  bool PosValidOnMap(PointDouble& NewPosition);
  bool PosValidTile(Tile* Tile);
  bool PosValidEntity(Entity* Entity);


  PointDouble _position;

  PointInt _center;

  PointDouble _speed;

  PointDouble _maxSpeed;

  PointDouble _accel;

  PointDouble _nextPosition;

  PointDouble _targetPosition;

  bool _isTargettingPosition;

  Rectangle _mask;
  Rectangle _drawRectangle;

  Rectangle _collisionMask;
  Rectangle _collisionOffset;

  // controls
  bool _moveLeft;
  bool _moveRight;
  bool _moveUp;
  bool _moveDown;
  PointDouble _moveDirection;

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
  GLuint _glTexture;
  Animation _animControl;
  int _currentFrameCol;
  int _currentFrameRow;
public:
  Behavior* b;

  bool _isTranslating;

};


class EntityCol {
  public:
    static std::map<std::pair<int, int>, EntityCol>    EntityColList;

    // rectangle of the intersection of bounding boxes for this collision
    Rectangle _rectangle;

  public:
    Entity* EntityA;

    Entity* EntityB;

    void updateContactSize();

  public:
    EntityCol();
};



#endif
