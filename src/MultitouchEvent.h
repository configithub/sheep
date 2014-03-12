#ifndef _MULTITOUCH_EVENT_
#define _MULTITOUCH_EVENT_

#include "FPS.h"
#include "Vectorial.h"
#include <vector>

class Multitouch {

  friend class MultitouchEvent;

  private:
  int _numberOfTouches;
  std::vector<PointInt> _points;

  public:

  Multitouch(int numberOfTouches=16) {
      std::cout << "init multitouch " << numberOfTouches << std::endl;
    _numberOfTouches = numberOfTouches;
    _points.reserve(_numberOfTouches);
    for(int i = 0; i <= _numberOfTouches; ++i) {
      _points.push_back(PointInt());
    }
  }

  void touchEvent(Uint8& which, Uint8& axis, Sint16& value) {
    if(axis==0)
      _points[which].setX(value);
    if(axis==1)
      _points[which].setY(value);
  }

  PointInt& getTouch(Uint8& which) {
    return _points[which];
  }

  PointInt& getTouch(int which) {
    return _points[which];
  }

  int getNumberOfTouches() { return _numberOfTouches; }

};

class MultitouchEvent : public Multitouch {
  private:
  // event validity delay, in ms
  int _validityDuration;
  std::vector<bool> _pointIsActive;
  std::vector<int> _pointActiveSince;

  public:

  static MultitouchEvent Controller;

  MultitouchEvent() {
    _validityDuration = 100;
    _pointIsActive.reserve(_numberOfTouches);
    _pointActiveSince.reserve(_numberOfTouches);
    for(int i = 0; i <= _numberOfTouches; ++i) {
      _pointIsActive.push_back(false);
      _pointActiveSince.push_back(0);
    }
  }

  void touch(Uint8& which, Uint8& axis, Sint16& value) {
    touchEvent(which, axis, value);
    _pointIsActive[which] = true;
    for(int i = 0; i <= _numberOfTouches; ++i) {
      _pointActiveSince[i] = 0;
    }
  }

  void OnLoop(int loopDuration) {
    for(int i = 0; i <= _numberOfTouches; ++i) {
      if(_pointIsActive[i]) {
          // add the duration of one frame to each counter
        _pointActiveSince[i] += loopDuration;
        if( _pointActiveSince[i] >  _validityDuration) {
          _pointIsActive[i] = false;
        }
      }
    }
  }

  void activePoint(Uint8& which) {
    _pointIsActive[which] = true;
  }

  bool isActivePoint(Uint8& which) {
    return _pointIsActive[which];
  }

  int getNumberOfActivePoints() {
    int result = 0;
    for(std::vector<bool>::const_iterator itActivePoint = _pointIsActive.begin();
      itActivePoint != _pointIsActive.end(); ++itActivePoint) {
      if(*itActivePoint) { ++result;}
    }
    return result;
  }

};



#endif
