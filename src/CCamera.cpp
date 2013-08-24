#include "CCamera.h"

CCamera CCamera::CameraControl;

CCamera::CCamera() {
  _position.set(0,0);
  _target = NULL;
  _isTranslating = false;

  TargetMode = TARGET_MODE_NORMAL;
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


void CCamera::translate(PointDouble& iPoint, double& dt) {
  if( *_target == iPoint) { return; }

  PointDouble dist;
  signedDistance(iPoint, *_target, dist);
  if(!_isTranslating) {
    _isTranslating = true;
    // dt is the time between two frames
    double entireTranslationTime = 5; // in ms
    _totalTranslationSteps = (int) (entireTranslationTime / dt);
    _translationStep = 1;

  }

  // std::cout << "dist: " << dist << std::endl;
  double moveX = dist.getX() / _totalTranslationSteps;
  double moveY = dist.getY() / _totalTranslationSteps;

  _target->getX() += moveX;
  _target->getY() += moveY;

  ++_translationStep;
  PointDouble newDist;
  distance(iPoint, *_target, newDist);
  if(newDist.modulus() < 2){
    _isTranslating = false;
    *_target = iPoint;
  }

}

