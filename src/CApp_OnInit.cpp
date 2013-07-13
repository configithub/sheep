#include "CApp.h"
#include <iostream>


bool CApp::OnInit() {

  // initialize SDL
  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) == -1 ) {
    return false;
  }
  if(! SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_OPENGL|SDL_DOUBLEBUF)  ) {
    return false;
  }
  // initialize OpenGL
#ifdef ANDROID
  int screenWidth = SDL_GetVideoInfo()->current_w;
  int screenHeight = SDL_GetVideoInfo()->current_h;
  glViewport(0, 0, screenWidth, screenHeight);
#else
  glViewport(0, 0, WWIDTH, WHEIGHT);
#endif

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();


  glOrtho(0.0f, WWIDTH, WHEIGHT, 0.0f, 1.0f, -1.0f);
  //glOrtho(0.0f, WWIDTH, 0.0f, WHEIGHT, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glDisable(GL_DEPTH_TEST);

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
