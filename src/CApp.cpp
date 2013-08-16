#include "CApp.h"

CApp theApp;
lua_State *luaState;

 std::map<int, CEntity> CApp::EntityPool;
 std::map<int, CFollower> CApp::SheepPool;
 std::map<int, Effect> CApp::EffectPool;
 std::map<int, Bomb> CApp::BombPool;
 std::map<int, Switch> CApp::SwitchPool;
 std::map<int, Saw> CApp::SawPool;
 std::map<int, Door> CApp::DoorPool;

 std::vector<CEntity*> CApp::Sheeps;
 int CApp::score=0; 
 int CApp::scoreTimer=0; 

CApp::CApp() {
    Running = true;
    activeSheep = 0;
    others_follow = false;
}

void CApp::OnGameOver() {
  //std::cout << "CApp: OnGameOver" << std::endl;
}

int CApp::OnExecute() {
    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;

    while(Running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }

        OnLoop();
        OnRender();
    }

    OnCleanup();

    return 0;
}

int main(int argc, char* argv[]) {
    // CApp theApp;

    return theApp.OnExecute();
}


