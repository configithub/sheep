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



class Rectangle {


};


#endif
