#include "Entity.h"

const int& Entity::getEntityId() const {
  return _entity_id;
}

void Entity::setEntityId(const int& iEntityId) {
  _entity_id = iEntityId;
}

void Entity::addComponent(const int iType, Component& iComponent) {
  _components.insert(std::make_pair(iType, &iComponent));
}

void Entity::removeComponent(const int iType) {
  _components.erase(iType);
}


