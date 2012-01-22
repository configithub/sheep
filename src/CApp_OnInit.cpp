#include "CApp.h"
#include <iostream>


bool CApp::OnInit() {

    // initialize SDL
    if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) == -1 ) {
    // if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    // create main surface
    // resolution, bit resolution, HW = hardware memory, double buffering prevent flickering
    // if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_HWSURFACE | SDL_DOUBLEBUF|SDL_HWACCEL)) == NULL) {
    if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_SWSURFACE )) == NULL) {
        return false;
    }

    float* centerX = new float(400);
    float* centerY = new float(240);

    SheepPool.reserve(1000);
    activeSheep = 1;


    /*if(Follower.OnLoad("./gfx/sheep2.png", 32, 32, 4) == false) {
      return false;
    }
    Follower.X = 100;
    Follower.Y = 100;
    Follower.id = 2;
    Sheeps.push_back(&Follower);*/



    if(Follower1.OnLoad("./gfx/sheep.png", 32, 32, 4) == false) {
      return false;
    }
    Follower1.X = 500;
    Follower1.Y = 100;
    Follower1.id = 1;

    Sheeps.push_back(&Follower1);


    /*if(Follower2.OnLoad("./gfx/sheep3.png", 32, 32, 4) == false) {
      return false;
    }
    Follower2.X = 350;
    Follower2.Y = 100;
    Follower2.id = 3;

    Sheeps.push_back(&Follower2);


    if(Follower3.OnLoad("./gfx/sheep4.png", 32, 32, 4) == false) {
      return false;
    }
    Follower3.X = 450;
    Follower3.Y = 100;
    Follower3.id = 4;

    Sheeps.push_back(&Follower3);*/

    for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
            itSheep != Sheeps.end(); ++itSheep) {
      CEntity::EntityList.push_back(*itSheep);
    }

    // to display number information, for now only framerate
    Surf_NumFont = CSurface::OnLoad("./gfx/font.png");
    //Surf_NumFont = CSurface::load_zoomed("./gfx/sheep.png", 0);

    CCamera::CameraControl.TargetMode = TARGET_MODE_CENTER;
    //CCamera::CameraControl.SetTarget(&Sheeps[activeSheep]->X, &Sheeps[activeSheep]->Y);
    CCamera::CameraControl.SetTarget( centerX, centerY);

    if(CArea::AreaControl.OnLoad("./maps/1.area") == false) {
        //std::cout << "area load unsuccessfull";
        return false;
    }

    SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

    return true;
}
