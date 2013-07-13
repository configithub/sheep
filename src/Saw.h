#ifndef SAW_H
#define SAW_H

#include "Behavior.h"
#include "CFollower.h"

class Saw : public Behavior {

  public:


    void OnRender();
    void OnCleanup();
    void OnAnimate();
    void OnInit();


};



#endif
