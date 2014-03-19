#ifndef _KINEMATIC_H_
#define _KINEMATIC_H_

#include "Component.h"

class Kinematic : public Component {
public:
  const float& getVx() const;
  void setVx(const float& iVx);  
  const float& getVy() const;
  void setVy(const float& iVy);  
private:
  float _vx;
  float _vy;
};

#endif
