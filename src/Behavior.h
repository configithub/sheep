#ifndef _BEHAVIOR_H_
#define _BEHAVIOR_H_

#include <vector>
#include <map>
#include <set>
#include <SDL.h>

class CEntity;

class Behavior {

  // Enums used in this class
  typedef enum {
    ENTITY_TYPE_GENERIC = 0,
    ENTITY_TYPE_PLAYER,
    SWITCH
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

  EN_EntityType getType() { return _type; }

  // constructor
  Behavior();

  // destroy 
  virtual ~Behavior();

  void setEntity(CEntity* aEntity) { e = aEntity; }

  virtual void OnInit();

  // decide whether this entity will be part of the next loop or not
  virtual void OnLoop();

  // animate the sprite of the entity
  virtual void OnAnimate();

  // render the entity on the display surface
  virtual void OnRender(SDL_Surface* ioSdlDisplaySurface);

  // operations to be done at clean up time : free memory
  virtual void OnCleanup();

  // when something dies
  virtual void kill();

  EN_EntityType _type;
  bool _dead;
  EN_EntityBehavior _behavior; // ghost or dead ...
  CEntity* e;

};


#endif
