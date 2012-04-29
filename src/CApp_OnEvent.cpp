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

void CApp::OnLButtonDown(int mX, int mY)
{

    //GoTo(mX,mY);
    //GoTo(mX,mY,2,1);
}

void CApp::OnLButtonUp(int mX, int mY)
{
    //StopGoTo(mX, mY);
    //GoTo(mX,mY,2,0);
}

void CApp::SelectHerdAtCoord(int x, int y) {

  DeselectAllSheeps();

  CFollower* NearestEntityFromMouse = NULL;

  int distance = 0;

  GetNearestEntityFromMouse(x, y, NearestEntityFromMouse, distance);

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
      if(abs((*itFollower)->X-(*itSheep)->X)+abs((*itFollower)->Y-(*itSheep)->Y) < delta && (*itSheep)->id == 0) {
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


void CApp::GetNearestEntityFromMouse(int x, int y, CFollower*& NearestEntityFromMouse, int& delta) {
  delta = 999999999;
  NearestEntityFromMouse = NULL;
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
            itSheep != Sheeps.end(); ++itSheep) {

    if(abs(x-(*itSheep)->X)+abs(y-(*itSheep)->Y) < delta) {
      delta =  abs(x-(*itSheep)->X)+abs(y-(*itSheep)->Y);
      NearestEntityFromMouse = *itSheep;
    }

  }

}

void CApp::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle)
{
    // std::cout << "mouseXY: " << mX << "," << mY << std::endl;
    //if(Left)
    //{



    if(MultitouchEvent::Controller.getNumberOfActivePoints() < 2) {
       SelectHerdAtCoord(mX, mY);
        GoTo(mX,mY);
    }



    //}
    //else
    //{
    //    StopGoTo(mX,mY);
    //}

    Mouse.setX(mX);
    Mouse.setY(mY);
}

void CApp::OnMultitouchEvent() {

   std::vector<Point> controls;
   controls.reserve(5);


   if(MultitouchEvent::Controller.getNumberOfActivePoints() == 2) {
      Point control1 = Point(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
      Point control2 = Point(MultitouchEvent::Controller.getTouch(2).getX(), MultitouchEvent::Controller.getTouch(2).getY());
      controls.push_back(control1);
      controls.push_back(control2);

      GoTo(controls);
    }
    if(MultitouchEvent::Controller.getNumberOfActivePoints() == 3) {
      Point control1 = Point(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
      Point control2 = Point(MultitouchEvent::Controller.getTouch(2).getX(), MultitouchEvent::Controller.getTouch(2).getY());
      Point control3 = Point(MultitouchEvent::Controller.getTouch(3).getX(), MultitouchEvent::Controller.getTouch(3).getY());
      controls.push_back(control1);
      controls.push_back(control2);
      controls.push_back(control3);

      GoTo(controls);
    }
    if(MultitouchEvent::Controller.getNumberOfActivePoints() == 4) {
      Point control1 = Point(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY());
      Point control2 = Point(MultitouchEvent::Controller.getTouch(2).getX(), MultitouchEvent::Controller.getTouch(2).getY());
      Point control3 = Point(MultitouchEvent::Controller.getTouch(3).getX(), MultitouchEvent::Controller.getTouch(3).getY());
      Point control4 = Point(MultitouchEvent::Controller.getTouch(4).getX(), MultitouchEvent::Controller.getTouch(4).getY());
      controls.push_back(control1);
      controls.push_back(control2);
      controls.push_back(control3);
      controls.push_back(control4);

      GoTo(controls);
    }

  //if(MultitouchEvent::Controller.getNumberOfActivePoints() == 3) {
  //  AddNewSheepInPool(activeSheep);
  //}
}

void CApp::OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value)
{
    std::cout << "which: " << which << std::endl;
    //std::cout << "axis: " << axis << std::endl;
    std::cout << "value: " << value << std::endl;

    //_multitouch.touchEvent(which,axis,value);
    MultitouchEvent::Controller.touch(which,axis,value);


    if(axis==0)
      JoyAxis.setX(value);
    if(axis==1)
      JoyAxis.setY(value);



    //AddNewSheepInPool(activeSheep);
}

void CApp::OnJoyButtonDown(Uint8 which,Uint8 button)
{
    std::cout << "joybutton down" << std::endl;
    MultitouchEvent::Controller.activePoint(which);
}

void CApp::OnJoyButtonUp(Uint8 which,Uint8 button)
{
    std::cout << "joybutton up" << std::endl;
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

    case SDLK_LEFT:
    {
        PushArrowLeftRight(false, true);
        break;
    }

    case SDLK_RIGHT:
    {
        PushArrowLeftRight(true, true);
        break;
    }

    case SDLK_UP:
    {
        PushArrowUp();
        break;
    }

    case SDLK_1:
    {
        SwitchActiveSheep(1);
        break;
    }

    case SDLK_2:
    {
        SwitchActiveSheep(2);
        break;
    }

    case SDLK_3:
    {
        SwitchActiveSheep(3);
        break;
    }

    case SDLK_4:
    {
        SwitchActiveSheep(4);
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



void CApp::GoTo(int x, int y)
{

    for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
            itSheep != Sheeps.end(); ++itSheep)
    {
        if((*itSheep)->id == activeSheep)
        {
            //std::cout << (*itSheep)->entityId % 2 << std::endl;
            (*itSheep)->GotoCommand = true;
            (*itSheep)->gotoX = x ;
            (*itSheep)->gotoY = y ;
        }
    }
}

void CApp::GoTo(std::vector<Point>& controls)
{

  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep)
  {

    if((*itSheep)->id == activeSheep )
    {
      (*itSheep)->GotoCommand = true;


      int delta = 999999999;
      Point NearestControl;
      for(std::vector<Point>::iterator itControl = controls.begin();
          itControl != controls.end(); ++itControl) {

        if(abs(itControl->getX()-(*itSheep)->X)+abs(itControl->getY()-(*itSheep)->Y) < delta) {
          delta = abs(itControl->getX()-(*itSheep)->X)+abs(itControl->getY()-(*itSheep)->Y);
          NearestControl = *itControl;
        }

      }
        (*itSheep)->gotoX = NearestControl.getX();
        (*itSheep)->gotoY = NearestControl.getY();
    }
  }
}

void CApp::StopGoTo(int x, int y)
{
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep)
  {
    //if((*itSheep)->id == activeSheep) {
    (*itSheep)->MoveLeft = false;
    (*itSheep)->MoveDown = false;
    (*itSheep)->MoveUp = false;
    (*itSheep)->MoveRight = false;
    //(*itSheep)->StopMove();
    //(*itSheep)->GotoCommand = false;

    //}
  }
}

void CApp::PushArrowLeftRight(bool right, bool down)
{
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep)
  {
    if((*itSheep)->id == activeSheep)
    {
      if(right) (*itSheep)->MoveRight = down;
      else (*itSheep)->MoveLeft = down;
    }
  }
}

void CApp::PushArrowUp()
{
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep)
  {
    if((*itSheep)->id == activeSheep)
    {
      //std::cout << "jump with id " << activeSheep <<  std::endl;
      (*itSheep)->Jump();
    }
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

  aStream << "./gfx/sheep"/* << sheepId <<*/ << ".png";

  char *fileName = (char*)aStream.str().c_str();

  std::cout << "add new sheep with id " << sheepId << " " << fileName <<  std::endl;

  if(newSheep.OnLoad("./gfx/sheep.png", 32, 32, 4) == false)
  {
    return false;
  }
  newSheep.X = X;
  newSheep.Y = Y;
  newSheep.id = sheepId;

  CEntity::EntityList.push_back(&newSheep);

  return true;

}



void CApp::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
  switch(sym)
  {
    case SDLK_LEFT:
      {
        PushArrowLeftRight(false, false);
        break;
      }

    case SDLK_RIGHT:
      {
        PushArrowLeftRight(true, false);
        break;
      }

    case SDLK_e:
      {

        break;
      }

      /*case SDLK_SPACE: {
        if(others_follow) {
        others_follow = false;
        OthersStopFollow();
        } else {
        others_follow = true;
        OthersFollow(activeSheep);

        }
        break;
        }*/


    default:
      {
      }
  }
}

void CApp::SwitchActiveSheep(int newActiveSheep)
{
  int oldActive = activeSheep;
  activeSheep = newActiveSheep;

  bool oldMoveLeft = false;
  bool oldMoveRight = false;
  // get old motion
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep)
  {
    if((*itSheep)->id == oldActive)
    {
      oldMoveLeft = (*itSheep)->MoveLeft;
      oldMoveRight = (*itSheep)->MoveRight;
      (*itSheep)->MoveLeft = false;
      (*itSheep)->MoveRight = false;
    }
  }

  // trigger the old motion in newly active sheeps
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep)
  {
    if((*itSheep)->id == activeSheep)
    {
      (*itSheep)->MoveLeft = oldMoveLeft;
      (*itSheep)->MoveRight = oldMoveRight;
    }
  }


  /*if(others_follow) {
    OthersFollow(activeSheep);
    }
    else {
    OthersStop();
    }*/

}

void CApp::OthersFollow(int followedSheep)
{
  int sheepNb=0;
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep)
  {
    if(sheepNb != followedSheep)
    {
      (*itSheep)->CanFollow = true;
      (*itSheep)->FollowedEntity = Sheeps[followedSheep];
    }
    else
    {
      (*itSheep)->CanFollow = false;
      (*itSheep)->FollowedEntity = NULL;
      //(*itSheep)->Stop();
    }
    ++sheepNb;
  }
}

void CApp::OthersStopFollow()
{
  int sheepNb=0;
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep)
  {
    if(sheepNb != activeSheep)
    {
      (*itSheep)->CanFollow = false;
      (*itSheep)->FollowedEntity = NULL;
      (*itSheep)->Stop();
    }
    ++sheepNb;
  }
}

void CApp::OthersStop()
{
  int sheepNb=0;
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep)
  {
    if(sheepNb != activeSheep)
    {
      //(*itSheep)->CanFollow = false;
      //(*itSheep)->FollowedEntity = NULL;
      (*itSheep)->Stop();
    }
    ++sheepNb;
  }

}

