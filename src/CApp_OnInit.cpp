#include "CApp.h"
#include <iostream>



bool CApp::OnInit() {

  // initialize SDL
  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) == -1 ) {
    return false;
  }
#ifdef ANDROID
  screenWidth = SDL_GetVideoInfo()->current_w;
  screenHeight = SDL_GetVideoInfo()->current_h;
#else
  screenWidth = WWIDTH;
  screenHeight = WHEIGHT;
#endif
  if(! SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_OPENGL|SDL_DOUBLEBUF)  ) {
    return false;
  }
  // initialize OpenGL
  glEnable(GL_BLEND);
#ifdef ANDROID
  glViewport(0, 0, screenWidth, screenHeight);
#else
  glViewport(0, 0, WWIDTH, WHEIGHT);
#endif

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();


  // glOrtho(0.0f, screenWidth, screenHeight, 0.0f, 1.0f, -1.0f);
  glOrtho(0.0f, WWIDTH, WHEIGHT, 0.0f, 1.0f, -1.0f);
  glMatrixMode(GL_MODELVIEW);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  std::cout << "glGetError(): " << glGetError() << std::endl;


  // joystick (multitouch) init
  SDL_JoystickEventState(SDL_ENABLE);
  SDL_Joystick* joystick = SDL_JoystickOpen(0);
  for(int i=0; i < SDL_NumJoysticks(); i++ ) {
    std::cout<<  SDL_JoystickName(i) << std::endl;
  }

  // reserve stuff
  CEntity::EntityList.reserve(5000);
  CEntity::NextEntityList.reserve(5000);
  CEntity::EntityListToRemove.reserve(5000);
  CApp::Sheeps.reserve(5000);
  _currentSpawningEntity = SHEEP;
  CSurface::OnInit();

  //EntityPool.reserve(1000);
  activeSheep = 1;

  _center = new PointDouble(0, 0);
  _nextCenter = new PointDouble(0, 0);

  CCamera::CameraControl.SetTarget(_center);

  if(Area::AreaControl.OnLoad("./maps/1.area") == false) {
    return false;
  }

  //PointDouble switchPosition(WWIDTH+60, WHEIGHT+90);
  PointDouble switchPosition(WWIDTH+190,WHEIGHT+ 160);
  int key = CEntity::CurrentEntityId;
  EntityPool[key].generateAtPos(switchPosition, SWITCH, this);
  EntityPool[key].b->setTriggerId(TRIGGER_ON_MOUSE);
  EntityPool[key].b->setActionId(SPAWN_BOMB);
  EntityPool[key].b->setSwitchId(ONE_SHOT);

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

  switchPosition = PointDouble(60, 55);
  key = CEntity::CurrentEntityId;
  EntityPool[key].generateAtPos(switchPosition, SWITCH, this);
  EntityPool[key].b->addTarget(doorId);
  EntityPool[key].b->setTriggerId(TRIGGER_ON_COLLISION);
  EntityPool[key].b->setSwitchId(NEED_CONTINUAL_PRESSURE);
  EntityPool[key].b->setActionId(BROADCAST);

  PointDouble sheepPosition(540, 80);
  key = CEntity::CurrentEntityId;
  EntityPool[key].generateAtPos(sheepPosition, SHEEP, this);

  //Rectangle aScreenRect(33, 33, WWIDTH-33, WHEIGHT-33);
  //int keyB = CEntity::CurrentEntityId;
  //BombPool[keyB].generateRandom(aScreenRect);

  // spawn entities from a lua script

  // initialize Lua 
  lua_State *L = lua_open();

  // load Lua base libraries 
  luaL_openlibs(L);

  // register our c++ function inside lua, now we can call it from a lua script
  lua_register(L, "spawn_entity", spawn_entity);

  // run the script where we call our c++ function
  luaL_dofile(L, "scripts/test.lua");

  // cleanup Lua 
  lua_close(L);


  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

  std::string levelName = "level";

  //Rectangle aScreenRect(33, 33, WWIDTH, WHEIGHT);
  //int key = CEntity::CurrentEntityId;
  //EntityPool[key].generateRandom(aScreenRect);
  //Sheeps.push_back(&CApp::EntityPool[key]);


  return true;
}
