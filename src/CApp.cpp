#include "CApp.h"

CApp theApp;
lua_State *luaState;

 std::map<int, Entity> CApp::EntityPool;
 std::map<int, Follower> CApp::SheepPool;
 std::map<int, Effect> CApp::EffectPool;
 std::map<int, Bomb> CApp::BombPool;
 std::map<int, Switch> CApp::SwitchPool;
 std::map<int, Saw> CApp::SawPool;
 std::map<int, Door> CApp::DoorPool;

 std::vector<Entity*> CApp::Sheeps;
 int CApp::score=0; 
 int CApp::scoreTimer=0; 

CApp::CApp() {
    Running = true;
    activeSheep = 0;
    others_follow = false;
  _background1_x = 0;
  _background2_x = 0;
  _background3_x = 0;
}

void CApp::OnGameOver() {
  //std::cout << "CApp: OnGameOver" << std::endl;
}


void CApp::OnInitLoop() {
  FPS::FPSControl.OnLoop();
  _hasGotMouseEventThisLoop = false;
}

int CApp::OnExecute() {
  if(OnInit() == false) {
    return -1;
  }

  SDL_Event Event;

  while(Running) {
    OnInitLoop();
    while(SDL_PollEvent(&Event)) {
      OnEvent(&Event);
    }
    OnGesture();
    OnLoop();
    OnRender();
  }

  OnCleanup();

  return 0;
}

void CApp::OnCameraContinuousTranslation() {
  // remove all entities in map 1,1
  for(std::vector<Entity*>::iterator itEntity = Entity::EntityList.begin(); 
    itEntity != Entity::EntityList.end(); ++itEntity) { 
    if( (*itEntity) != NULL) {
      if((*itEntity)->getPosition().getX() < WWIDTH) {
        (*itEntity)->removeAtNextLoop() = true;
      }else{
        (*itEntity)->getNextPosition().setX((*itEntity)->getPosition().getX() - WWIDTH);
        (*itEntity)->getPosition().setX((*itEntity)->getPosition().getX() - WWIDTH);
      }
    }
  }

  // switch map 1,1 with map 1,2
  Map map00 = *Area::AreaControl.GetMap(0,0);
  Area::AreaControl.SetMap(0,0, Area::AreaControl.GetMap(801,0));
  Area::AreaControl.SetMap(801,0, &map00); 
}


int main(int argc, char* argv[]) {
  // CApp theApp;
  return theApp.OnExecute();
}


