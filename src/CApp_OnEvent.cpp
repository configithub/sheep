#include "CApp.h"
#include <iostream>
#include <sstream>
#include "Gesture.h"


void CApp::OnEvent(SDL_Event* Event)
{
  // handles the event to quit the application
  /*if(Event->type == SDL_QUIT) {
    Running = false;
    }*/

  // handle all events through inheritance of the CEvent class
  CEvent::OnEvent(Event);
  OnMultitouchEvent();
}

// handles app exit
void CApp::OnExit()
{
  // override of virtual method CEvent::OnExit
  Running = false;
}

void CApp::SelectHerdAtCoord(PointDouble& point) {

  DeselectAllSheeps();

  CEntity* NearestEntityFromMouse = NULL;

  int distance = 0;

  GetNearestEntityFromMouse(point, NearestEntityFromMouse, distance);

  if(NearestEntityFromMouse == NULL) { return; }
  std::vector<CEntity*> herd;
  herd.push_back(NearestEntityFromMouse);
  getActiveGroup(herd);

}

void CApp::getActiveGroup(std::vector<CEntity*>& ioGroup) {

  if(ioGroup.empty()) { return; }

  std::vector<CEntity*> resultGroup;
  int delta = 80;

  for(std::vector<CEntity*>::iterator itFollower = ioGroup.begin();
      itFollower != ioGroup.end(); ++itFollower) {
    (*itFollower)->_selection = 1;
  }

  for(std::vector<CEntity*>::iterator itFollower = ioGroup.begin();
      itFollower != ioGroup.end(); ++itFollower) {
    for(std::vector<CEntity*>::iterator itSheep = Sheeps.begin();
        itSheep != Sheeps.end(); ++itSheep) {
      PointDouble distPoint; distance( (*itFollower)->getPosition(), (*itSheep)->getPosition(), distPoint);
      if( distPoint.modulus() < delta && (*itSheep)->_selection == 0) {
        resultGroup.push_back(*itSheep);
      }
    }

  }

  getActiveGroup(resultGroup);

}

void CApp::DeselectAllSheeps() {
  for(std::vector<CEntity*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    (*itSheep)->_selection = 0;
  }
}

void CApp::SelectAllSheeps() {
  for(std::vector<CEntity*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    (*itSheep)->_selection = 1;
  }
}

void CApp::SelectAllSheepsInCurrentRoom() {
  CMap* currentRoom = Area::AreaControl.GetMap(_center->getX()+5, _center->getY()+5);
  // get tile at center (where the camera stands)

  _nbSelectedEntity = 0;
  for(std::vector<CEntity*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    CMap* currentSheepRoom = Area::AreaControl.GetMap( (*itSheep)->getPosition().getX(), (*itSheep)->getPosition().getY());
    if(currentRoom == currentSheepRoom) { // sheep is in currentRoom
      (*itSheep)->_selection = 1;
      ++_nbSelectedEntity;
    }else{
      (*itSheep)->_selection = 0;
    }
  }
}


void CApp::GetNearestEntityFromMouse(PointDouble& point, CEntity*& NearestEntityFromMouse, int& delta) {
  delta = 999999999;
  NearestEntityFromMouse = NULL;
  for(std::vector<CEntity*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    PointDouble distPoint; distance( point,(*itSheep)->getPosition(), distPoint);
    int dist = distPoint.modulus();
    if( dist < delta) { delta = dist; NearestEntityFromMouse = *itSheep; }
  }
}


void CApp::triggerSwitchesOnTouch() {

  // TODO, only impact switches in current room
  for(std::map<int,Switch>::iterator itSwitch = SwitchPool.begin(); 
      itSwitch != SwitchPool.end(); ++itSwitch) { 
    itSwitch->second.triggerOnTouch(Mouse);    
  } 

}


void CApp::OnResize(int w, int h) {
  // reload open gl stuff

  //#ifdef ANDROID
  //if(! SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_OPENGL|SDL_DOUBLEBUF | SDL_FULLSCREEN)  ) {
  //#else
  //if(! SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_OPENGL|SDL_DOUBLEBUF)  ) {
  //#endif
  //}
  // initialize OpenGL
  glEnable(GL_BLEND);
#ifdef ANDROID
  glViewport(0, 0, screenWidth, screenHeight);
#else
  glViewport(0, 0, WWIDTH, WHEIGHT);
#endif

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();


  // glOrtho(0.0f, screenWidth, screenHeight, 0.0f, 1.0f, -1.0f);
  glOrtho(0.0f, WWIDTH, WHEIGHT, 0.0f, 1.0f, -1.0f);
  glMatrixMode(GL_MODELVIEW);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  std::cout << "glGetError(): " << glGetError() << std::endl;
  CSurface::OnInit();
  if(Area::AreaControl.OnLoad("./maps/1.area") == false) {
    return ;
  }
}
/*
   void CApp::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle)
   {
#ifndef ANDROID
if(!Left) { return; }
#endif
Mouse.setX(mX+_center->getX());
Mouse.setY(mY+_center->getY());
int tileTypeOnMouse = Area::AreaControl.GetTile(Mouse.getX(), Mouse.getY())->TypeID;
//std::cout << "bop" << std::endl;
if(tileTypeOnMouse == TILE_TYPE_NORMAL) {
if(MultitouchEvent::Controller.getNumberOfActivePoints() < 2) {
//SelectHerdAtCoord(Mouse);
SelectAllSheepsInCurrentRoom();
GoTo(Mouse);
triggerSwitchesOnTouch();
}
}else{
switch(tileTypeOnMouse) {
case TRANSLATE_LEFT:
if(!CCamera::CameraControl.isTranslating()) {
//std::cout << "Left" << std::endl;
_nextCenter->set(_center->getX()-WWIDTH, _center->getY());
}
break;
case TRANSLATE_RIGHT:
if(!CCamera::CameraControl.isTranslating()) {
//std::cout << "Right" << std::endl;
_nextCenter->set(_center->getX()+WWIDTH, _center->getY());
}
break;
case TRANSLATE_UP:
if(!CCamera::CameraControl.isTranslating()) {
//std::cout << "Up" << std::endl;
_nextCenter->set(_center->getX(), _center->getY()-WHEIGHT);
}
break;
case TRANSLATE_DOWN:
if(!CCamera::CameraControl.isTranslating()) {
//std::cout << "Down" << std::endl;
_nextCenter->set(_center->getX(), _center->getY()+WHEIGHT);
}
break;
}
}
} */

void CApp::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle) {
#ifndef ANDROID
  if(!Left) { return; }
#endif
  Mouse.setX(mX+_center->getX()-WWIDTH/2);
  Mouse.setY(mY+_center->getY()-WHEIGHT/2);
  //Mouse.setX(mX+_center->getX());
  //Mouse.setY(mY+_center->getY());
  _hasGotMouseEventThisLoop = true;
  triggerSwitchesOnTouch();
  checkTranslation();
}


void CApp::checkTranslation() {
  int tileTypeOnMouse = Area::AreaControl.GetTile(Mouse.getX(), Mouse.getY())->TypeID;
  if(tileTypeOnMouse != TILE_TYPE_NORMAL) {
    _hasGotMouseEventThisLoop = false; // ugly
    switch(tileTypeOnMouse) {
      case TRANSLATE_LEFT:
        if(!CCamera::CameraControl.isTranslating()) {
          _nextCenter->set(_center->getX()-WWIDTH, _center->getY());
        }
        break;
      case TRANSLATE_RIGHT:
        if(!CCamera::CameraControl.isTranslating()) {
          _nextCenter->set(_center->getX()+WWIDTH, _center->getY());
        }
        break;
      case TRANSLATE_UP:
        if(!CCamera::CameraControl.isTranslating()) {
          _nextCenter->set(_center->getX(), _center->getY()-WHEIGHT);
        }
        break;
      case TRANSLATE_DOWN:
        if(!CCamera::CameraControl.isTranslating()) {
          _nextCenter->set(_center->getX(), _center->getY()+WHEIGHT);
        }
        break;
    }
  }
}


void CApp::OnMultitouchEvent() {

  controls.clear();
  controls.reserve(5);

  PointDouble absCenter = PointDouble(WWIDTH/2, WHEIGHT/2);

  /*if(MultitouchEvent::Controller.getNumberOfActivePoints() == 1) {
    PointDouble control1 = PointDouble(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
    Mouse.setX(control1.getX());
    Mouse.setY(control1.getY());
    SelectHerdAtCoord(Mouse);
    GoTo(Mouse);
    }*/
  if(MultitouchEvent::Controller.getNumberOfActivePoints() == 2) {
    PointDouble control1 = PointDouble(MultitouchEvent::Controller.getTouch(0).getX(), MultitouchEvent::Controller.getTouch(0).getY());
    PointDouble control2 = PointDouble(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
    control1 += *_center - absCenter;
    control2 += *_center - absCenter;
    controls.push_back(control1);
    controls.push_back(control2);
    _hasGotMouseEventThisLoop = true;
  }
  if(MultitouchEvent::Controller.getNumberOfActivePoints() == 3) {
    PointDouble control1 = PointDouble(MultitouchEvent::Controller.getTouch(0).getX(), MultitouchEvent::Controller.getTouch(0).getY());
    PointDouble control2 = PointDouble(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
    PointDouble control3 = PointDouble(MultitouchEvent::Controller.getTouch(2).getX(), MultitouchEvent::Controller.getTouch(2).getY());
    control1 += *_center - absCenter;
    control2 += *_center - absCenter;
    control3 += *_center - absCenter;
    controls.push_back(control1);
    controls.push_back(control2);
    controls.push_back(control3);
    _hasGotMouseEventThisLoop = true;
  }
  if(MultitouchEvent::Controller.getNumberOfActivePoints() == 4) {
    PointDouble control1 = PointDouble(MultitouchEvent::Controller.getTouch(0).getX(), MultitouchEvent::Controller.getTouch(0).getY());
    PointDouble control2 = PointDouble(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
    PointDouble control3 = PointDouble(MultitouchEvent::Controller.getTouch(2).getX(), MultitouchEvent::Controller.getTouch(2).getY());
    PointDouble control4 = PointDouble(MultitouchEvent::Controller.getTouch(3).getX(), MultitouchEvent::Controller.getTouch(3).getY());
    control1 += *_center - absCenter;
    control2 += *_center - absCenter;
    control3 += *_center - absCenter;
    control4 += *_center - absCenter;
    controls.push_back(control1);
    controls.push_back(control2);
    controls.push_back(control3);
    controls.push_back(control4);
    _hasGotMouseEventThisLoop = true;
  }
  if(MultitouchEvent::Controller.getNumberOfActivePoints() > 1) {
    //GoTo(controls);
    _hasGotMouseEventThisLoop = true;
  }

}

void CApp::OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value)
{
  std::cout << "which: " << which << std::endl;
  std::cout << "value: " << value << std::endl;

  //MultitouchEvent::Controller.touch(which,axis,value);

}

void CApp::OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel) {
  Uint8 xAxis = 0;
  Uint8 yAxis = 1;
  MultitouchEvent::Controller.touch(ball,xAxis,xrel);
  MultitouchEvent::Controller.touch(ball,yAxis,yrel);
}

void CApp::OnJoyButtonDown(Uint8 which,Uint8 button) {
  std::cout << "joybutton down" << std::endl;
  MultitouchEvent::Controller.activePoint(button);
}

void CApp::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
  switch(sym) {

    case SDLK_SPACE:
      {
        break;
      }


    case SDLK_f: // back key
      {
        exit(1);
        break;
      }


    default: {}
  }
}



void CApp::GoTo(PointDouble& point) {

  for(std::vector<CEntity*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->_selection == activeSheep) {
      (*itSheep)->isTargettingPosition(true);
      (*itSheep)->getTargetPosition().set(point.getX(), point.getY());
    }
  }
}

void CApp::GoTo(std::vector<PointDouble>& controls) {
  for(std::vector<CEntity*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {

    if((*itSheep)->_selection == activeSheep )
    {
      (*itSheep)->isTargettingPosition(true);


      int delta = 999999999;
      PointDouble NearestControl;
      for(std::vector<PointDouble>::iterator itControl = controls.begin();
          itControl != controls.end(); ++itControl) {

        PointDouble distPoint; distance( *itControl, (*itSheep)->getPosition(), distPoint);
        int dist = distPoint.modulus();
        if( dist < delta) {
          delta = dist;
          NearestControl = *itControl;
        }

      }
      (*itSheep)->getTargetPosition().set(NearestControl.getX(), NearestControl.getY());

    }
  }
}


void CApp::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch(sym)
  {
    case SDLK_e:
      {
        std::cout << "spawn" << std::endl;
        Rectangle aScreenRect(33, 33, WWIDTH-99, WHEIGHT-99);
        for (int i = 0; i < 1; i++) {
          int key = CEntity::CurrentEntityId; 
          CApp::EntityPool[key].generateRandom(aScreenRect, _currentSpawningEntity, this);
          CApp::EntityPool[key].b->setAttribute(PLAYER_CONTROLLED, 1);
          Sheeps.push_back(&CApp::EntityPool[key]);
        }
        break;
      }
    case SDLK_c:
      {
        //_nextCenter->set(0, 0);
        _currentSpawningEntity = (EN_EntityType)((int)_currentSpawningEntity+1);
        if(_currentSpawningEntity == NUM_TYPE) {
          _currentSpawningEntity = SHEEP;
        }
        std::cout << "Spawning: " << _currentSpawningEntity << std::endl;
        break;
      }

    case SDLK_d:
      {
        //_nextCenter->set(WWIDTH, 0);
        _currentSpawningEntity = (EN_EntityType)((int)_currentSpawningEntity-1);
        if(_currentSpawningEntity < 0) {
          _currentSpawningEntity = (EN_EntityType)((int)NUM_TYPE-1);
        }
        std::cout << "Spawning: " << _currentSpawningEntity << std::endl;
        break;
      }

    default:
      {
      }
  }
}
