#include "CApp.h"
#include <iostream>
#include "Level.h"

SDL_Surface* CApp::Surf_NumFont = NULL;
SDL_Surface* CApp::Surf_NumFontBomb = NULL;
SDL_Surface* CApp::Surf_GameOver = NULL;
SDL_Surface* CApp::Surf_Score = NULL;

bool CApp::OnInit() {

  // initialize SDL
  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) == -1 ) {
    // if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  }

  for(int i=0; i < SDL_NumJoysticks(); i++ )
  {
    std::cout<<  SDL_JoystickName(i) << std::endl;
  }

  SDL_JoystickEventState(SDL_ENABLE);

  /*for(int i = 0; i <= 16; ++i) {
    SDL_Joystick* newjoystick = SDL_JoystickOpen(i);
    _sdlJoysticks.push_back(newjoystick);
    }*/

  SDL_Joystick* joystick = SDL_JoystickOpen(0);


  //JoyAxis.setX( SDL_JoystickNumAxes(joystick) );
  //JoyAxis.setY( SDL_JoystickNumBalls(joystick) );

  //joystickRef.

  // create main surface
  // resolution, bit resolution, HW = hardware memory, double buffering prevent flickering
  // if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_HWSURFACE | SDL_DOUBLEBUF|SDL_HWACCEL)) == NULL) {
  if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_SWSURFACE )) == NULL) {
    return false;
  }


  //EntityPool.reserve(1000);
  activeSheep = 1;

  CSurface::OnInit();

  /*if(Follower.OnLoad("./gfx/sheep6.png", 32, 32, 4) == false) {
    return false;
    }
    Follower.getPosition().set(100, 100);
    Follower.id = 1;

    Sheeps.push_back(&Follower);

    for(std::vector<CFollower*>::iterator itSheep = Sheeps.begin();
    itSheep != Sheeps.end(); ++itSheep) {
    CEntity::EntityList.push_back(*itSheep);
    }*/

  // to display number information, for now only framerate
  if(Surf_NumFont == NULL) {
    Surf_NumFont = CSurface::OnLoad("./gfx/font.png");
  }
  if(Surf_NumFontBomb == NULL) {
    Surf_NumFontBomb = CSurface::OnLoad("./gfx/font1.png");
  }
  if(Surf_GameOver == NULL) {
    Surf_GameOver = CSurface::OnLoad("./gfx/gameover.png");
  }
  if(Surf_Score == NULL) {
    Surf_Score = CSurface::OnLoad("./gfx/scorefont.png");
  }
  //Surf_NumFont = CSurface::load_zoomed("./gfx/sheep.png", 0);

  //float* centerX = new float(WWIDTH/2);
  //float* centerY = new float(WHEIGHT/2);
  //PointDouble* center = new PointDouble(WWIDTH/2, WHEIGHT/2);
  //PointDouble* center = new PointDouble(WWIDTH, 0);
  _center = new PointDouble(0, 0);

  //CCamera::CameraControl.TargetMode = TARGET_MODE_CENTER;
  //CCamera::CameraControl.SetTarget(&Sheeps[activeSheep]->X, &Sheeps[activeSheep]->Y);
  CCamera::CameraControl.SetTarget(_center);

  if(CArea::AreaControl.OnLoad("./maps/1.area") == false) {
    //std::cout << "area load unsuccessfull";
    return false;
  }

  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

  std::string levelName = "level";

  //Rectangle aScreenRect(33, 33, WWIDTH, WHEIGHT);
  //int key = CEntity::CurrentEntityId;
  //EntityPool[key].generateRandom(aScreenRect);
  //Sheeps.push_back(&CApp::EntityPool[key]);

  //Level::LevelInstance.next(Sheeps, levelName, 0, 2000, 1000, 2);

  return true;
  }
