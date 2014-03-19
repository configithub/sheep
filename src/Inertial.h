#ifndef _INERTIAL_H_
#define _INERTIAL_H_

#include "Component.h"

class Inertial : public Component {
public:
  const float& getAx() const;
  void setAx(const float& iAx);  
  const float& getAy() const;
  void setAy(const float& iAy);  
private:
  float _ax;
  float _ay;
};

#endif
