#include "Positionable.h"

const float& Positionable::getX() const {
  return _x;
}

void Positionable::setX(const float& iX) {
  _x = iX;
}

const float& Positionable::getY() const {
  return _y;
}

void Positionable::setY(const float& iY) {
  _y = iY;
}
