#ifndef _VECTORIAL_H_
#define _VECTORIAL_H_

#include <iostream>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

template <class T> class Point {

  public:

  Point<T>() {
    _x = 0;
    _y = 0;
  }

  Point<T>(T iX, T iY) {
    _x = iX;
    _y = iY;
  }

  T& getX() { return _x; }
  T& getY() { return _y; }
  void setX(T iX) { _x = iX; }
  void setY(T iY) { _y = iY; }
  void set(T iX, T iY) { _x = iX; _y = iY; }



  Point<T> operator +(Point<T> b) {
    return Point<T>( _x+b.getX(), _y+b.getY() );
  }

  void operator +=(Point<T> b) {
    set( _x+b.getX(), _y+b.getY() );
  }

  bool operator ==(Point<T> b) {
    return _x == b.getX() && _y == b.getY() ;
  }

  Point<T> operator -(Point<T> b) {
    return Point<T>( _x-b.getX(), _y-b.getY() );
  }

  void operator -=(Point<T> b) {
    set( _x-b.getX(), _y-b.getY() );
  }



  // product with a scalar
  Point<T> operator *(T scalar) {
    return Point<T>( _x*scalar, _y*scalar);
  }


  void operator =(Point<T>& b) {
    _x = b.getX(); _y = b.getY();
  }

  void cap(Point<T>& b) {
    if(abs(_x) > abs(b.getX())) { _x = sgn<T>(_x)*b.getX(); }
    if(abs(_y) > abs(b.getY())) { _y = sgn<T>(_y)*b.getY(); }
  }

  T modulus() {
    return abs(_x) + abs(_y);
  }

  private:
  T _x;
  T _y;

};

template <class T, class V> void distance(Point<T>& a, Point<T>& b, Point<V>& oResult) {
  oResult.set( abs(a.getX()-b.getX()), abs(a.getY()-b.getY()) );
}

template <class T, class V> void signedDistance(Point<T>& a, Point<T>& b, Point<V>& oResult) {
  oResult.set( (a.getX()-b.getX()), (a.getY()-b.getY()) );
}

template <class T> T scalarProduct(Point<T>& a, Point<T>& b) {
  return a.getX()*b.getX() + a.getY()*b.getY();
}

typedef Point<int> PointInt;
typedef Point<double> PointDouble;

class Rectangle {
  public:


  Rectangle(): _width(0), _height(0) {}
  Rectangle(int iWidth, int iHeight) {
    _width = iWidth;
    _height = iHeight;
  }

  Rectangle(int iX, int iY, int iWidth, int iHeight) : _corner(iX,iY) {

    _width = iWidth;
    _height = iHeight;
  }

  void operator =(Rectangle& b) {
    _corner = b.getCorner();
    _width = b.getWidth();
    _height = b.getHeight();
  }

  Point<int>& getCorner() { return _corner; }
  int& getWidth() { return _width; }
  int& getHeight() { return _height; }
  void setWidth(int& iWidth) { _width = iWidth; }
  void setHeight(int& iHeight) { _height = iHeight; }
  void setWidth(const int& iWidth) { _width = iWidth; }
  void setHeight(const int& iHeight) { _height = iHeight; }
  void set(PointInt& iCorner, int iWidth, int iHeight) {
    _corner = iCorner; _width = iWidth; _height = iHeight; }

    void set(int x, int y, int iWidth, int iHeight) {
    _corner.set(x,y); _width = iWidth; _height = iHeight; }

  Point<int>& getCenter() {
    _center.set(_corner.getX()+_width/2, _corner.getY()+_height/2);
    return _center;
  }

  bool isVoid() {
    return (_width*_height) == 0;
  }

  void intersect(Rectangle& b, Rectangle& result) {

    result.set(0,0,0,0);

    int left1 = _corner.getX();
    int left2 = b.getCorner().getX();
    int right1 = left1 + _width -1;
    int right2 = left2 + b.getWidth() - 1;
    int top1 = _corner.getY();
    int top2 = b.getCorner().getY();
    int bottom1 = top1 + _height - 1;
    int bottom2 = top2 + b.getHeight() - 1;



    if (bottom1 < top2) return ;
    if (top1 > bottom2) return ;
    if (right1 < left2) return ;
    if (left1 > right2) return ;


    result.setWidth( std::min( abs(right1 - left2), abs(right2 - left1)) );
    result.setHeight( std::min( abs(bottom1 - top2), abs(bottom2 - top1)) );


    if(result.isVoid()) {
      result.setWidth(0);
      result.setHeight(0);
    }

    return  ;
  }

  private:
  Point<int> _corner;
  Point<int> _center;
  int _width;
  int _height;

};


#endif
