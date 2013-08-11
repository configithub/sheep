#ifndef _BEHAVIOR_H_
#define _BEHAVIOR_H_

#include <vector>
#include <map>
#include <set>
#include <SDL.h>
#include <string>
#include <iostream>
#include "Vectorial.h"

class CEntity;

class Value {
  public:
    enum Type { NIL, INT, DOUBLE, STRING };

    Value() : type(NIL) {}
    Value(int i) : type(INT) { val.ival = i; }
    Value(double d) : type(DOUBLE) { val.dval = d; }
    Value(const char* str);
    Value(const std::string& str);
    ~Value();

    Value(const Value& v); 
    Value& operator=(const Value& v); 

    void assign(double d); 
    void assign(int i); 
    void assign(const char* s); 

    Type    get_type() const { return type; }
    double  get_double() const;
    int get_int() const;
    const char* get_string() const;
  private:
    void clear();

    // Variables
    Type type;
    union {
      double dval;
      int ival;
      char* str;
    } val;
};  

std::ostream& operator<<(std::ostream& os, const Value& val);

bool        to_bool(const Value &v);
int         to_int(const Value &v);
double      to_double(const Value &v);
const char *to_string(const Value &v);
// Direction   to_direction (const Value &v);

typedef enum {
  TRIGGER_ON_MOUSE,
  TRIGGER_ON_COLLISION
} EN_TriggerType;

typedef enum {
  SPAWN_BOMB,
  BROADCAST,
  CALLBACK
} EN_Action;

typedef enum {
  TARGET,
  ACTION, 
  LOCK,
  TRIGGER_TYPE,
  SWITCH_TYPE,
  PLAYER_CONTROLLED,
  DEAD
} EN_Attribute;

class Behavior {

  // Enums used in this class


  // Friend classes
  friend class CFollower;
  friend class Effect;
  friend class Switch;
  friend class Bomb;

  // Public members
  public:


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

  // trigger reception from a switch for instance
  virtual void OnTriggeredAction(int id=0);

  // render the entity on the display surface
  virtual void OnRender();

  // operations to be done at clean up time : free memory
  virtual void OnCleanup();

  // when something dies
  virtual void kill();

  const Value& getAttribute(EN_Attribute key) { return _attributes[key]; } 
  void setAttribute(EN_Attribute key, Value iValue) { _attributes[key] = iValue; }

  bool _dead;
  CEntity* e;

  private:

  std::map<EN_Attribute, Value> _attributes;

};


#endif
