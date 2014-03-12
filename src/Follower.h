#ifndef _Follower_H_
#define _Follower_H_

#include "Behavior.h"

class Follower : public Behavior {
  public:
    Follower();

    void OnAnimate();
      void OnAnimateStaticBackground();
      void OnAnimateMovingBackground();

    void kill();

    void OnCleanup();


  private:
    int _isActive;
};

#endif
