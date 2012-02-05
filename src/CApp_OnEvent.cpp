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

void CApp::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle)
{
    // std::cout << "mouseXY: " << mX << "," << mY << std::endl;
    //if(Left)
    //{
    if(MultitouchEvent::Controller.getNumberOfActivePoints() < 2) {
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

   if(MultitouchEvent::Controller.getNumberOfActivePoints() == 2) {


      GoTo(MultitouchEvent::Controller.getTouch(1).getX(), MultitouchEvent::Controller.getTouch(1).getY(), 2, 0);
      GoTo(MultitouchEvent::Controller.getTouch(2).getX(), MultitouchEvent::Controller.getTouch(2).getY(), 2, 1);
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

/*void CApp::OnJoyHat(Uint8 which,Uint8 axis,Sint16 value)
{
    std::cout << "which: " << which << std::endl;
    //std::cout << "axis: " << axis << std::endl;
    std::cout << "value: " << value << std::endl;

    if( axis == 0)
    {
        std::cout << "axis: " << 0 << std::endl;
    }

    if( axis == 1)
    {
        std::cout << "axis: " << 1 << std::endl;
    }
    //JoyAxis.setX((int)value);
    //JoyAxis.setY((int)axis);


}*/

/*void CApp::OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel) {
    //JoyAxis.setX((int)xrel);
    //JoyAxis.setY((int)yrel);
    //AddNewSheepInPool(activeSheep);
}*/

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





    default:
    {
    }
    }
}

void CApp::GoToIsometric(int x, int y)
{

    double delta = 999999999;

    CEntity* NearestEntityFromMouse = NULL;
    for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
            itSheep != Sheeps.end(); ++itSheep)
    {
        if((*itSheep)->id == activeSheep)
        {
            if(abs(x-(*itSheep)->X)+abs(y-(*itSheep)->Y) < delta)
            {
                delta =  abs(x-(*itSheep)->X)+abs(y-(*itSheep)->Y);
                NearestEntityFromMouse = *itSheep;
            }
        }
    }


    for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
            itSheep != Sheeps.end(); ++itSheep)
    {
        if((*itSheep)->id == activeSheep)
        {
            (*itSheep)->GotoCommand = true;
            (*itSheep)->gotoX = x + (*itSheep)->X - NearestEntityFromMouse->X;
            (*itSheep)->gotoY = y + (*itSheep)->Y - NearestEntityFromMouse->Y;
        }
    }
}

void CApp::GoToHybrid(int x, int y)
{

    double delta = 999999999;

    CEntity* NearestEntityFromMouse = NULL;
    for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
            itSheep != Sheeps.end(); ++itSheep)
    {
        if((*itSheep)->id == activeSheep)
        {
            if(abs(x-(*itSheep)->X)+abs(y-(*itSheep)->Y) < delta)
            {
                delta =  abs(x-(*itSheep)->X)+abs(y-(*itSheep)->Y);
                NearestEntityFromMouse = *itSheep;
            }
        }
    }

    if(delta < 50)
    {
        StopGoTo(x,y);
    }
    else
    {
        for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
                itSheep != Sheeps.end(); ++itSheep)
        {
            if((*itSheep)->id == activeSheep)
            {
                (*itSheep)->GotoCommand = true;
                (*itSheep)->gotoX = x ;
                (*itSheep)->gotoY = y ;
            }
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

void CApp::GoTo(int x, int y, int subgroupNb, int subGroup)
{

    for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
            itSheep != Sheeps.end(); ++itSheep)
    {
        if( (*itSheep)->id == activeSheep && (*itSheep)->entityId % subgroupNb == subGroup)
        {
            (*itSheep)->GotoCommand = true;
            (*itSheep)->gotoX = x ;
            (*itSheep)->gotoY = y ;
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

    case SDLK_SPACE:
    {
        AddNewSheepInPool(activeSheep);
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

