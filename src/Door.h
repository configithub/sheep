#ifndef DOOR_H
#define DOOR_H

#include "Behavior.h"
#include "CFollower.h"

typedef enum {
  CLOSED = 0,
  CLOSING,
  OPEN,
  OPENING
} EN_DoorState;

class Door : public Behavior {

  public:

    Door() : _iterationNb(0), _maxIterationNb(1), _doorState(CLOSED) { }

    void OnRender();
    void OnCleanup();
    void OnAnimate();
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
