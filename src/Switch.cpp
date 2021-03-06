#include "Switch.h"
#include "CApp.h"


void Switch::OnRender() {
  if(_isPushed) {
    int currentTime = SDL_GetTicks();
    if(currentTime - _startTime < _delay) {
      // switch did not go back to unpushed state yet 
    }else{
      if(getAttribute(SWITCH_TYPE).get_int() == ONE_SHOT) {
        _isPushed = false;
      }
    }
  }
}

void Switch::OnLoop() {
  if(getAttribute(SWITCH_TYPE).get_int() == NEED_CONTINUAL_PRESSURE) {
    _isPushed = false;
  }
}

void Switch::OnCleanup() {
  CApp::SwitchPool.erase(e->_entityId); 
}

void Switch::OnAnimate() {

  e->_currentFrameCol = 0;

  int initialState = e->_animControl.MinFrames;

  if (!_isPushed) {
    e->_animControl.MinFrames = 0;
    e->_animControl.MaxFrames = 0;
  }else{
    e->_animControl.MinFrames = 1;
    e->_animControl.MaxFrames = 1;
  }

  if(getAttribute(SWITCH_TYPE).get_int() == NEED_CONTINUAL_PRESSURE) {
    if(initialState == 1 && e->_animControl.MinFrames == 0) {
      switch(getAttribute(ACTION).get_int()) {
        case 0:
          spawnBombInRoom();
          break;
        case 1:
          broadcastToTargets(CHANGE_DOOR_STATE);
          //broadcastToTargets(CLOSE_DOOR);
          break;
      }
    }
    if(initialState == 0 && e->_animControl.MinFrames == 1) {
      switch(getAttribute(ACTION).get_int()) {
        case 0:
          spawnBombInRoom();
          break;
        case 1:
          broadcastToTargets(CHANGE_DOOR_STATE);
          //broadcastToTargets(OPEN_DOOR);
          break;
      }
    }
  }

}

// action 0
/*void Switch::spawnBombInRoom() {
  // get suitable rectangle for the current room
  Rectangle aScreenRect(e->getPosition().getX()+10,
      e->getPosition().getY(), 10, 10);

  // spawn a bomb randomly in this rectangle
  int key = Entity::CurrentEntityId;
  CApp::EntityPool[key].generateRandom(aScreenRect, BOMB, e->_parent); 
}*/

void Switch::spawnBombInRoom() {
  Value nil;
  // run the script where we call our c++ function
  CallFunc(luaState, "spawn_bombs", nil); 

}

// action 1
void Switch::broadcastToTargets(int id) {
  CApp::EntityPool[getAttribute(TARGET).get_int()].b->OnTriggeredAction(id);

}

void Switch::OnTriggeredAction(int id) {
  if(id != getAttribute(TRIGGER_TYPE).get_int()) { return; }
  if(_isPushed) { return; }
  _isPushed = true;
  _startTime = SDL_GetTicks();   

  if(getAttribute(SWITCH_TYPE).get_int() != NEED_CONTINUAL_PRESSURE) {
    switch(getAttribute(ACTION).get_int()) {
      case 0:
        spawnBombInRoom();
        break;
      case 1:
        broadcastToTargets();
        break;
    }
  }

}

void Switch::triggerOnTouch(PointDouble& iMouse) { 
  if(getAttribute(TRIGGER_TYPE).get_int() != TRIGGER_ON_MOUSE) { return; }
  PointDouble distance;
  Entity::dist(*e, iMouse, distance);

  if ( distance.getX() < 32 && distance.getY() < 32 ) {
    OnTriggeredAction(TRIGGER_ON_MOUSE);
  }

}




