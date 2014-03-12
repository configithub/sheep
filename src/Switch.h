#ifndef SWITCH_H
#define SWITCH_H

#include "Follower.h"
#include "Vectorial.h"

typedef enum {
  ONE_SHOT_STAY,
  ONE_SHOT,
  NEED_CONTINUAL_PRESSURE
} EN_SwitchType;

class Switch : public Behavior {

  public:

    Switch(): _isPushed(false), _delay(400) {
    }

    void OnRender();

    void OnAnimate();

    void OnCleanup();

    void OnLoop();

    // action 0
    void spawnBombInRoom();

    void setDelay(int& iDelay) { _delay = iDelay; } 

    // trigger 0 
    void triggerOnTouch(PointDouble& iMouse);

    void OnTriggeredAction(int id = 0);

    void generateAtPos(PointDouble& iPosition);

    void broadcastToTargets(int id = 2);

  private:

    bool _isPushed; // the button is pushed 

    int _startTime;   
    int _delay; // delay at which the button can be repushed after trigger, in ms

};



#endif
