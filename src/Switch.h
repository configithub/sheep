#ifndef SWITCH_H
#define SWITCH_H

#include "CFollower.h"
#include "Vectorial.h"

class Switch : public Behavior {

  public:

    Switch(): _isPushed(false), _delay(400), _actionId(0), _triggerId(0) {
      _type = SWITCH;
    }

    void OnRender();

    void OnAnimate();

    void OnCleanup();

    // action 0
    void spawnBombInRoom();

    void setDelay(int& iDelay) { _delay = iDelay; } 

    // trigger 0 
    void triggerOnTouch(PointDouble& iMouse);

    void setActionId(int id) { _actionId = id; } 

    void trigger();

    bool getTriggerId() { return _triggerId; }

    void generateAtPos(PointDouble& iPosition);

    void addTarget(int iTarget) { _targets.push_back(iTarget); }

    void broadcastToTargets();

  private:

    bool _isPushed; // the button is pushed 

    int _startTime;   
    int _delay; // delay at which the button can be repushed after trigger, in ms

    int _actionId; // defines which action will be triggered on activation
    // 0 : spawns a bomb at random in the room
    // 1 : broadcast to the targets to call their OnActivate function

    int _triggerId; // defines what will trigger the action
    // 0 : activation on touch
    // 1 : activation on sheep collision

    // define the objetcs that will be affected by the switch
    std::vector<int> _targets;
    

};



#endif
