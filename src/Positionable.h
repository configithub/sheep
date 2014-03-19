#ifndef _POSITIONABLE_H_
#define _POSITIONABLE_H_

#include "Component.h"

class Positionable : public Component {
public:
  const float& getX() const;
  void setX(const float& iX);  
  const float& getY() const;
  void setY(const float& iY);  
private:
  float _x;
  float _y;
};

#endif
