#include "Component.h"

const int& Component::getEntityId() const {
  return _entity_id;
}

void Component::setEntityId(const int& iEntityId) {
  _entity_id = iEntityId;
}

void Component::setEntityId(const int iEntityId) {
  _entity_id = iEntityId;
}

void Component::activate() {
  _active = true;
}

void Component::deactivate() {
  _active = false;
}
