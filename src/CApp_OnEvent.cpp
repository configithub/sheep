#include "CApp.h"
#include <iostream>
#include <sstream>

void CApp::OnEvent(SDL_Event* Event) {
  // handles the event to quit the application
  /*if(Event->type == SDL_QUIT) {
      Running = false;
  }*/

  // handle all events through inheritance of the CEvent class
  CEvent::OnEvent(Event);
}

// handles app exit
void CApp::OnExit() {
  // override of virtual method CEvent::OnExit

  Running = false;
}

void CApp::OnLButtonDown(int mX, int mY) {
  GoTo(mX, mY);
}

void CApp::OnLButtonUp(int mX, int mY) {
  StopGoTo(mX, mY);
}

void CApp::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle) {
   // std::cout << "mouseXY: " << mX << "," << mY << std::endl;
  GoTo(mX,mY);
}

void CApp::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
  switch(sym) {


     case SDLK_SPACE: {
        AddNewSheepInPool(activeSheep);
        break;
      }

    case SDLK_LEFT: {
        PushArrowLeftRight(false, true);
        break;
      }

    case SDLK_RIGHT: {
        PushArrowLeftRight(true, true);
        break;
      }

    case SDLK_UP: {
        PushArrowUp();
        break;
      }

    case SDLK_1: {
        SwitchActiveSheep(1);
        break;
      }

    case SDLK_2: {
        SwitchActiveSheep(2);
        break;
      }

    case SDLK_3: {
        SwitchActiveSheep(3);
        break;
      }

    case SDLK_4: {
        SwitchActiveSheep(4);
        break;
      }





    default: {
      }
  }
}

void CApp::GoTo(int x, int y) {
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->id == activeSheep) {
        (*itSheep)->GotoCommand = true;
        (*itSheep)->gotoX = x;
        (*itSheep)->gotoY = y;
    }
  }
}

void CApp::StopGoTo(int x, int y) {
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->id == activeSheep) {

        (*itSheep)->GotoCommand = false;

    }
  }
}

void CApp::PushArrowLeftRight(bool right, bool down) {
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->id == activeSheep) {
      if(right) (*itSheep)->MoveRight = down;
      else (*itSheep)->MoveLeft = down;
    }
  }
}

void CApp::PushArrowUp() {
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->id == activeSheep) {
        //std::cout << "jump with id " << activeSheep <<  std::endl;
      (*itSheep)->Jump();
    }
  }
}

void CApp::SwitchActiveSheep(int newActiveSheep) {
  int oldActive = activeSheep;
  activeSheep = newActiveSheep;

  bool oldMoveLeft = false;
  bool oldMoveRight = false;
  // get old motion
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->id == oldActive) {
      oldMoveLeft = (*itSheep)->MoveLeft;
      oldMoveRight = (*itSheep)->MoveRight;
      (*itSheep)->MoveLeft = false;
      (*itSheep)->MoveRight = false;
    }
  }

  // trigger the old motion in newly active sheeps
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if((*itSheep)->id == activeSheep) {
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

void CApp::OthersFollow(int followedSheep) {
  int sheepNb=0;
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if(sheepNb != followedSheep) {
      (*itSheep)->CanFollow = true;
      (*itSheep)->FollowedEntity = Sheeps[followedSheep];
    } else {
      (*itSheep)->CanFollow = false;
      (*itSheep)->FollowedEntity = NULL;
      //(*itSheep)->Stop();
    }
    ++sheepNb;
  }
}

void CApp::OthersStopFollow() {
  int sheepNb=0;
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if(sheepNb != activeSheep) {
      (*itSheep)->CanFollow = false;
      (*itSheep)->FollowedEntity = NULL;
      (*itSheep)->Stop();
    }
    ++sheepNb;
  }
}

void CApp::OthersStop() {
  int sheepNb=0;
  for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
      itSheep != Sheeps.end(); ++itSheep) {
    if(sheepNb != activeSheep) {
      //(*itSheep)->CanFollow = false;
      //(*itSheep)->FollowedEntity = NULL;
      (*itSheep)->Stop();
    }
    ++sheepNb;
  }

}

bool CApp::AddNewSheepInPool(int sheepId) {
  sheepId = (sheepId%5);



  CFollower newFollower;
  SheepPool.push_back(newFollower);
  CFollower& newSheep = SheepPool.back();
  Sheeps.push_back(&newSheep);

  std::stringstream aStream;

  aStream << "./gfx/sheep"/* << sheepId <<*/ ".png";

  char *fileName = (char*)aStream.str().c_str();

  std::cout << "add new sheep with id " << sheepId << " " << fileName <<  std::endl;

  if(newSheep.OnLoad(fileName, 32, 32, 4) == false) {
    return false;
  }
  newSheep.X = 300;
  newSheep.Y = 300;
  newSheep.id = sheepId;

  CEntity::EntityList.push_back(&newSheep);

  return true;

}



void CApp::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
  switch(sym) {
    case SDLK_LEFT: {
        PushArrowLeftRight(false, false);
        break;
      }

    case SDLK_RIGHT: {
        PushArrowLeftRight(true, false);
        break;
      }

    case SDLK_SPACE: {
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


    default: {
      }
  }
}

