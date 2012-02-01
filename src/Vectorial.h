#ifndef _VECTORIAL_H_
#define _VECTORIAL_H_

#include <iostream>

class Point {
  private:
  int X;
  int Y;

  public:

  Point() {
    X = 0;
    Y = 0;
  }

  Point(int iX, int iY) {
    X = iX;
    Y = iY;
  }

  int getX() { return X; }
  int getY() { return Y; }
  void setX(int iX) { X = iX; }
  void setY(int iY) { Y = iY; }

};

class Multitouch {
  private:
  int _numberOfTouches;
  std::vector<Point> _points;



  public:

  Multitouch(int numberOfTouches=16) {
      std::cout << "init multitouch " << numberOfTouches << std::endl;
    _numberOfTouches = numberOfTouches;
    for(int i = 0; i <= _numberOfTouches; ++i) {
      _points.push_back(Point());
    }
  }

  void touchEvent(Uint8& which, Uint8& axis, Sint16& value) {
    if(axis==0)
      _points[which].setX(value);
    if(axis==1)
      _points[which].setY(value);
  }

  Point& getTouch(Uint8& which) {
    return _points[which];
  }

  int getNumberOfTouches() { return _numberOfTouches; }

};

class Rectangle {


};


#endif
