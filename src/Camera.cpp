#include "Camera.h"
#include "CApp.h"
#include "FPS.h"

Camera Camera::CameraControl;

Camera::Camera() {
  _parent = NULL;
  _position.set(0,0);
  // where we look at
  _target = NULL;
  // to know if the camera is currently translating
  _isTranslating = false;
  // the step at which we are during the translation
  _translationStep = 0;
  // camera mode
  TargetMode = TARGET_MODE_NORMAL;
  // default position for a continuously translating screen (e.g. first screen)
  _origin.set(WWIDTH/2, WHEIGHT/2); 
  // distance to move at the next frame
  _moveX = 0;
  _moveY = 0;
}

void Camera::OnMove(int MoveX, int MoveY) {
  _position.getX() += MoveX;
  _position.getY() += MoveY;
}

int Camera::GetX() {
  if(_target != NULL) {
    if(TargetMode == TARGET_MODE_CENTER) {
      return _target->getX() - (WWIDTH / 2);
    }

    return _target->getX();
  }

  return _position.getX();
}

int Camera::GetY() {
  if(_target != NULL) {
    if(TargetMode == TARGET_MODE_CENTER) {
      return _target->getY() - (WHEIGHT / 2);
    }

    return _target->getY();
  }

  return _position.getY();
}


void Camera::translate(PointDouble& iPoint, double translationSpeed, double& dt) {
  if(*_target == iPoint) { return; }

  //PointDouble dist;
  //signedDistance(iPoint, *_target, dist);
  if(!_isTranslating) {
    _isTranslating = true;
    // dt is the time between two frames
  }

  _moveX = translationSpeed * dt;
  _moveY = 0.0 ;

  _target->getX() += _moveX;
  _target->getY() += _moveY;

  PointDouble newDist;
  distance(iPoint, *_target, newDist);
  if(iPoint.getX() <= _target->getX()){
    *_target = iPoint;
    _isTranslating = false;
  }

}
