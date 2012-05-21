#include "CApp.h"
#include <iostream>
#include <sstream>


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

  CFollower* NearestEntityFromMouse = NULL;

  int distance = 0;

  GetNearestEntityFromMouse(point, NearestEntityFromMouse, distance);

  std::vector<CFollower*> herd;
  herd.push_back(NearestEntityFromMouse);
  getActiveGroup(herd);

}

void CApp::getActiveGroup(std::vector<CFollower*>& ioGroup) {

  if(ioGroup.empty()) { return; }

  std::vector<CFollower*> resultGroup;
  int delta = 80;

  for(std::vector<CFollower*>::iterator itFollower = ioGroup.begin();
      itFollower != ioGroup.end(); ++itFollower) {
    (*itFollower)->id = 1;
  }


  for(std::vector<CFollower*>::iterator itFollower = ioGroup.begin();
      itFollower != ioGroup.end(); ++itFollower) {
    for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
        itSheep != Sheeps.end(); ++itSheep) {
      PointDouble distPoint; distance( (*itFollower)->getPosition(), (*itSheep)->getPosition(), distPoint);
      if( distPoint.modulus() < delta && (*itSheep)->id == 0) {
        resultGroup.push_back(*itSheep);
      }
    }

  }

  getActiveGroup(resultGroup);

}

void CApp::DeselectAllSheeps() {
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    (*itSheep)->id = 0;
  }
}


void CApp::GetNearestEntityFromMouse(PointDouble& point, CFollower*& NearestEntityFromMouse, int& delta) {
  delta = 999999999;
  NearestEntityFromMouse = NULL;
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    PointDouble distPoint; distance( point,(*itSheep)->getPosition(), distPoint);
    int dist = distPoint.modulus();
    if( dist < delta) { delta = dist; NearestEntityFromMouse = *itSheep; }
  }
}

void CApp::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle)
{
  Mouse.setX(mX);
  Mouse.setY(mY);

    std::cout << mX << ", " << mY << std::endl;
  if(MultitouchEvent::Controller.getNumberOfActivePoints() < 2) {

    SelectHerdAtCoord(Mouse);
    GoTo(Mouse);
  }
}

void CApp::OnMultitouchEvent() {

  std::vector<PointDouble> controls;
  controls.reserve(5);


  /*if(MultitouchEvent::Controller.getNumberOfActivePoints() == 1) {
    PointDouble control1 = PointDouble(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
    controls.push_back(control1);
  }*/
  if(MultitouchEvent::Controller.getNumberOfActivePoints() == 2) {
    PointDouble control1 = PointDouble(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
    PointDouble control2 = PointDouble(MultitouchEvent::Controller.getTouch(2).getX(), MultitouchEvent::Controller.getTouch(2).getY());
    controls.push_back(control1);
    controls.push_back(control2);
  }
  if(MultitouchEvent::Controller.getNumberOfActivePoints() == 3) {
    PointDouble control1 = PointDouble(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
    PointDouble control2 = PointDouble(MultitouchEvent::Controller.getTouch(2).getX(), MultitouchEvent::Controller.getTouch(2).getY());
    PointDouble control3 = PointDouble(MultitouchEvent::Controller.getTouch(3).getX(), MultitouchEvent::Controller.getTouch(3).getY());
    controls.push_back(control1);
    controls.push_back(control2);
    controls.push_back(control3);
  }
  if(MultitouchEvent::Controller.getNumberOfActivePoints() == 4) {
    PointDouble control1 = PointDouble(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
    PointDouble control2 = PointDouble(MultitouchEvent::Controller.getTouch(2).getX(), MultitouchEvent::Controller.getTouch(2).getY());
    PointDouble control3 = PointDouble(MultitouchEvent::Controller.getTouch(3).getX(), MultitouchEvent::Controller.getTouch(3).getY());
    PointDouble control4 = PointDouble(MultitouchEvent::Controller.getTouch(4).getX(), MultitouchEvent::Controller.getTouch(4).getY());
    controls.push_back(control1);
    controls.push_back(control2);
    controls.push_back(control3);
    controls.push_back(control4);

  }
  if(MultitouchEvent::Controller.getNumberOfActivePoints() > 1) {
    GoTo(controls);
  }

}

void CApp::OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value)
{
  std::cout << "which: " << which << std::endl;
  std::cout << "value: " << value << std::endl;

  MultitouchEvent::Controller.touch(which,axis,value);

  if(axis==0)
    JoyAxis.setX(value);
  if(axis==1)
    JoyAxis.setY(value);

}

void CApp::OnJoyButtonDown(Uint8 which,Uint8 button)
{
  std::cout << "joybutton down" << std::endl;
  MultitouchEvent::Controller.activePoint(which);
}

void CApp::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch(sym)
  {


    case SDLK_SPACE:
      {
        //AddNewSheepInPool(activeSheep);
        break;
      }

    case SDLK_e:
      {
        AddNewSheepInPool(activeSheep);
        break;
      }

    default:
      {
      }
  }
}



void CApp::GoTo(PointDouble& point) {

  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->id == activeSheep) {
      (*itSheep)->isTargettingPosition(true);
      (*itSheep)->getTargetPosition().set(point.getX(), point.getY());
    }
  }
}

void CApp::GoTo(std::vector<PointDouble>& controls) {
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep)
  {

    if((*itSheep)->id == activeSheep )
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

void CApp::StopGoTo() {
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    (*itSheep)->StopMove();
  }
}


bool CApp::AddNewSheepInPool(int sheepId, double X, double Y)
{
  sheepId = (sheepId%5);



  CFollower newFollower;
  SheepPool.push_back(newFollower);
  CFollower& newSheep = SheepPool.back();
  Sheeps.push_back(&newSheep);

  std::stringstream aStream;

  aStream << "./gfx/sheep6"/* << sheepId <<*/ << ".png";

  char *fileName = (char*)aStream.str().c_str();

  std::cout << "add new sheep with id " << sheepId << " " << fileName <<  std::endl;

  if(newSheep.OnLoad("./gfx/sheep6.png", 32, 32, 4) == false)
  {
    return false;
  }
  newSheep.getPosition().set(X, Y);
  newSheep.id = sheepId;

  CEntity::EntityList.push_back(&newSheep);

  return true;

}



void CApp::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch(sym)
  {
    case SDLK_e:
      {

        break;
      }

    default:
      {
      }
  }
}
