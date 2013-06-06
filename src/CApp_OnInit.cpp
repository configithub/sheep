#include "CApp.h"
#include <iostream>

SDL_Surface* CApp::Surf_NumFont = NULL;
SDL_Surface* CApp::Surf_NumFontBomb = NULL;
SDL_Surface* CApp::Surf_GameOver = NULL;
SDL_Surface* CApp::Surf_Score = NULL;

bool CApp::OnInit() {

  // initialize SDL
  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) == -1 ) {
    return false;
  }

  CEntity::EntityList.reserve(5000);
  CEntity::NextEntityList.reserve(5000);
  CEntity::EntityListToRemove.reserve(5000);
  CApp::Sheeps.reserve(5000);
  _currentSpawningEntity = SHEEP;
  for(int i=0; i < SDL_NumJoysticks(); i++ ) {
    std::cout<<  SDL_JoystickName(i) << std::endl;
  }

  SDL_JoystickEventState(SDL_ENABLE);
  SDL_Joystick* joystick = SDL_JoystickOpen(0);

  // create main surface
  // resolution, bit resolution, HW = hardware memory, double buffering prevent flickering
  // if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_HWSURFACE | SDL_DOUBLEBUF|SDL_HWACCEL)) == NULL) {
  if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_SWSURFACE )) == NULL) {
    return false;
  }
  CSurface::OnInit();

  //EntityPool.reserve(1000);
  activeSheep = 1;


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

  _center = new PointDouble(0, 0);
  _nextCenter = new PointDouble(0, 0);

  CCamera::CameraControl.SetTarget(_center);

  if(CArea::AreaControl.OnLoad("./maps/1.area") == false) {
    return false;
  }

  //PointDouble switchPosition(WWIDTH+60, WHEIGHT+90);
  PointDouble switchPosition(WWIDTH+190,WHEIGHT+ 160);
  int key = CEntity::CurrentEntityId;
  EntityPool[key].generateAtPos(switchPosition, SWITCH, this);

  PointDouble sawPosition(100,WHEIGHT+ 60);
  key = CEntity::CurrentEntityId;
  EntityPool[key].generateAtPos(sawPosition, SAW, this);

  sawPosition = PointDouble(100,WHEIGHT+ 100);
  key = CEntity::CurrentEntityId;
  EntityPool[key].generateAtPos(sawPosition, SAW, this);

  sawPosition = PointDouble(140,WHEIGHT+ 140);
  key = CEntity::CurrentEntityId;
  EntityPool[key].generateAtPos(sawPosition, SAW, this);

  PointDouble doorPosition(304, 80);
  key = CEntity::CurrentEntityId;
  EntityPool[key].generateAtPos(doorPosition, DOOR, this);
  int doorId = key;

  switchPosition = PointDouble(260, 55);
  key = CEntity::CurrentEntityId;
  EntityPool[key].generateAtPos(switchPosition, SWITCH, this);
  EntityPool[key].b->addTarget(doorId);
  EntityPool[key].b->setActionId(1);


  //Rectangle aScreenRect(33, 33, WWIDTH-33, WHEIGHT-33);
  //int keyB = CEntity::CurrentEntityId;
  //BombPool[keyB].generateRandom(aScreenRect);

  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

  std::string levelName = "level";

  //Rectangle aScreenRect(33, 33, WWIDTH, WHEIGHT);
  //int key = CEntity::CurrentEntityId;
  //EntityPool[key].generateRandom(aScreenRect);
  //Sheeps.push_back(&CApp::EntityPool[key]);


  return true;
  }
