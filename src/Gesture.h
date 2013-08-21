#ifndef _GESTURE_H_
#define _GESTURE_H_

#include "Vectorial.h"
#include <vector>

typedef enum {
  VOID,
  INCOMING,
  READY_FOR_HARVEST
} EN_GestureState;

class GestureController {

  public:

    GestureController();


    void reset();

    void update(PointDouble& iPosition);

    void OnLoop(bool hasGotMouseEvent, PointDouble& mouse, int& loopDuration);

    void harvest(std::vector<PointDouble>& ioPositions, int nbPoints=0);

  private:

    std::vector<PointDouble> _rawPositions;
    PointDouble _currentPosition;
    double _offset;
    EN_GestureState _state;
    int _survivalDelay;
    int _currentDelay;

};

static GestureController GestureControllerInstance;

#endif
