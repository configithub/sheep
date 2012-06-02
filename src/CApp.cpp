#include "CApp.h"

 std::map<int, CFollower> CApp::EntityPool;
 std::map<int, Effect> CApp::EffectPool;

CApp::CApp() {
    Surf_Display = NULL;
    Surf_Test = NULL;
    Surf_NumFont = NULL;
    Surf_NumFontBomb = NULL;
    Running = true;
    activeSheep = 0;
    others_follow = false;
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


