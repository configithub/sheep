#include "CEntity.h"

std::map<std::pair<int, int>, CEntityCol> CEntityCol::EntityColList;

CEntityCol::CEntityCol() {
    this->EntityA = NULL;
    this->EntityB = NULL;
}

void CEntityCol::updateContactSize() {

  EntityA->updateCollisionMask();
  EntityB->updateCollisionMask();

  EntityA->getCollisionMask().intersect(EntityB->getCollisionMask(), _rectangle);

}
