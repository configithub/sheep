#ifndef DOOR_H
#define DOOR_H

#include "Behavior.h"
#include "Follower.h"

typedef enum {
  CLOSED = 0,
  CLOSING,
  OPEN,
  OPENING
} EN_DoorState;

typedef enum {
  OPEN_DOOR,
  CLOSE_DOOR,
  CHANGE_DOOR_STATE
} EN_DoorAction;

class Door : public Behavior {

  public:

    Door() : _iterationNb(0), _maxIterationNb(1), _doorState(CLOSED) { }

    void OnRender();
    void OnCleanup();
    void OnAnimate(); // set the animation parameters : MinFrame, MaxFrame, and FrameCol
    void OnInit();
   
    void OnTriggeredAction(int id); 
    void Open();
    void Close();

  private:

    int _iterationNb;
    int _maxIterationNb;

    EN_DoorState _doorState;
};



#endif
