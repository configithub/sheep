#include "CCamera.h"
#include "CApp.h"
#include "CFPS.h"

CCamera CCamera::CameraControl;

CCamera::CCamera() {
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

void CCamera::OnMove(int MoveX, int MoveY) {
  _position.getX() += MoveX;
  _position.getY() += MoveY;
}

int CCamera::GetX() {
  if(_target != NULL) {
    if(TargetMode == TARGET_MODE_CENTER) {
      return _target->getX() - (WWIDTH / 2);
    }

    return _target->getX();
  }

  return _position.getX();
}

int CCamera::GetY() {
  if(_target != NULL) {
    if(TargetMode == TARGET_MODE_CENTER) {
      return _target->getY() - (WHEIGHT / 2);
    }

    return _target->getY();
  }

  return _position.getY();
}


void CCamera::translate(PointDouble& iPoint, double& dt, bool cont) {
  if(*_target == iPoint) { return; }

  //PointDouble dist;
  //signedDistance(iPoint, *_target, dist);
  if(!_isTranslating) {
    _isTranslating = true;
    // dt is the time between two frames
  }

  double translationSpeed = 5;

  _moveX = translationSpeed * dt;
  _moveY = 0.0 ;

  _target->getX() += _moveX;
  _target->getY() += _moveY;

  if(CFPS::FPSControl.one_second) {
    std::cout << "_moveX: " << _moveX << std::endl;
    std::cout << "dt: " << dt << std::endl;
  }

  PointDouble newDist;
  distance(iPoint, *_target, newDist);
  // if(newDist.modulus() < 2){
  if(iPoint.getX() <= _target->getX()){
    if(!cont) {
      *_target = iPoint;
      _isTranslating = false;
    }else{
      _isTranslating = false;
      _moveX = _origin.getX() - _target->getX();
      _moveY = _origin.getY() - _target->getY();
      *_target = _origin;
      _parent->OnCameraContinuousTranslation();
    }
  }

}
