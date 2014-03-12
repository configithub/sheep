#include "Entity.h"

std::map<std::pair<int, int>, EntityCol> EntityCol::EntityColList;

EntityCol::EntityCol() {
    this->EntityA = NULL;
    this->EntityB = NULL;
}

void EntityCol::updateContactSize() {

  EntityA->updateCollisionMask();
  EntityB->updateCollisionMask();

  EntityA->getCollisionMask().intersect(EntityB->getCollisionMask(), _rectangle);

}
