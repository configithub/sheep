#include "Inertial.h"

const float& Inertial::getAx() const {
  return _ax;
}

void Inertial::setAx(const float& iAx) {
  _ax = iAx;
}

const float& Inertial::getAy() const {
  return _ay;
}

void Inertial::setAy(const float& iAy) {
  _ay = iAy;
}
