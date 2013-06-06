#ifndef DOOR_H
#define DOOR_H

#include "Behavior.h"
#include "CFollower.h"

class Door : public Behavior {

  public:


    Door() : _iterationNb(0), _maxIterationNb(1), _isClose(true), _isOpen(false), _isClosing(false), _isOpening(false) {
    }

    void OnRender(SDL_Surface* ioSdlDisplaySurface);
    void OnCleanup();
    void OnAnimate();
    void OnInit();
   
    void OnTriggeredAction(); 
    void Open();
    void Close();

  private:

    int _iterationNb;
    int _maxIterationNb;

    bool _isClosing;
    bool _isOpening;
    bool _isOpen;
    bool _isClose;

};



#endif
