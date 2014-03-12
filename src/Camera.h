#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include <SDL.h>

#include "Define.h"
#include "Vectorial.h"

enum {
  TARGET_MODE_NORMAL = 0,
  TARGET_MODE_CENTER
};

class CApp;

class Camera {
  public:
    static Camera CameraControl;

    int TargetMode;

    Camera();

    void OnMove(int MoveX, int MoveY);

    void setParent(CApp* iParent) { _parent = iParent; }

    PointDouble& GetPosition() { return _position; }
    int GetX();
    int GetY();

    void SetPos(int X, int Y) { _position.set(X,Y); }
    void SetPosition(PointDouble& iPosition) { _position = iPosition; }

    void SetTarget(PointDouble* iPositionPtr) { _target = iPositionPtr; }

    PointDouble& getTarget() { return *_target; }

    void translate(PointDouble& iPoint, double translationSpeed, double& dt);

    bool& isTranslating() { return _isTranslating; }

    double& getMoveX() { return _moveX; }
    double& getMoveY() { return _moveY; }

  private:
    PointDouble _position;

    PointDouble* _target;

    PointDouble _origin;

    bool _isTranslating;
    int _translationStep;
    int _totalTranslationSteps;

    double _moveX;
    double _moveY;

    CApp* _parent;

};

#endif
