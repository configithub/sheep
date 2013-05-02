#include "CApp.h"

 std::map<int, CEntity> CApp::EntityPool;
 std::map<int, CFollower> CApp::SheepPool;
 std::map<int, Effect> CApp::EffectPool;
 std::map<int, Bomb> CApp::BombPool;
 std::map<int, Switch> CApp::SwitchPool;

 std::vector<CEntity*> CApp::Sheeps;
 int CApp::score=0; 
 int CApp::scoreTimer=0; 

CApp::CApp() {
    Surf_Display = NULL;
    Surf_Test = NULL;
    Surf_NumFont = NULL;
    Surf_NumFontBomb = NULL;
    Surf_GameOver = NULL;
    Running = true;
    activeSheep = 0;
    others_follow = false;
}

void CApp::OnGameOver() {
  //std::cout << "CApp: OnGameOver" << std::endl;
  CSurface::OnDraw(Surf_Display, Surf_GameOver, 1, 85,1 ,1 ,319,69);
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
    CApp theApp;

    return theApp.OnExecute();
}


