#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <map>
#include "Engine.h"
#include "Component.h"

class Entity {
public:
  const int& getEntityId() const;
  void setEntityId(const int& iEntityId);  
  void addComponent(const int iType, Component& iComponent);
  void removeComponent(const int iType);

private:
  Entity(Engine iEngine) : _entity_id(-1), _engine(&iEngine) {}
  std::map<int, Component*> _components;
  int _entity_id;
  Engine* _engine;
};

#endif
