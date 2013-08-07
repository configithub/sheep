#ifndef _CFollower_H_
#define _CFollower_H_

#include "Behavior.h"

class CFollower : public Behavior {
  public:
    CFollower(): _isActive(0) {};

    void OnAnimate();

    void kill();

    void OnCleanup();

    void setActionId(int id) { _isActive = id; }
    int& getActionId() { return _isActive; }

  private:
    int _isActive;
};

#endif
