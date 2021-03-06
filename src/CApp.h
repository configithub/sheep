#ifndef _CAPP_H_
#define _CAPP_H_

#include <SDL.h>
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "Texture.h"
#include "Event.h"
#include "Animation.h"
#include "Entity.h"
#include "Define.h"
#include "Area.h"
#include "Camera.h"
#include "Follower.h"
#include "Vectorial.h"
#include "MultitouchEvent.h"
#include "FPS.h"
#include "Effect.h"
#include "Bomb.h"
#include "Switch.h"
#include "Saw.h"
#include "Door.h"



enum Error {
  NO_LUAERROR = 0,
  ERRRUN,
  ERRFILE,
  ERRSYNTAX,
  ERRMEM,
  ERRERR
};

extern lua_State *luaState;
class CApp : public Event {
  private:
    bool            Running;


    std::vector<SDL_Joystick*> _sdlJoysticks;

    int activeSheep;

    bool others_follow;

    //Follower Follower;

    EN_EntityType _currentSpawningEntity;

    PointDouble Mouse;
    std::vector<PointDouble> controls; // from multitouch
    bool _hasGotMouseEventThisLoop;


  public:

    void initLuaState();

    int screenWidth;
    int screenHeight;

    //static PointDouble& getMouse() { return Mouse; }
    double _translationSpeed;
    double _background1_speed;
    int _background1_x;
    double _background2_speed;
    int _background2_x;
    double _background3_speed;
    int _background3_x;

    EN_EntityType& getCurrentSpawningEntity() { _currentSpawningEntity; }
    void setCurrentSpawningEntity(EN_EntityType& iType) { _currentSpawningEntity = iType; }

    static int score;
    static int scoreTimer;
    static void updateScore();

    PointDouble* _center;
    PointDouble* _nextCenter;

    static std::vector<Entity*> Sheeps;
    int _nbSelectedEntity;
    static void print_num(SDL_Surface *dst, SDL_Surface *font, int x, int y, float value);

    // all entities
    static std::map<int, Entity> EntityPool;

    // behavior by type
    static std::map<int, Follower> SheepPool;
    static std::map<int, Switch> SwitchPool;
    static std::map<int, Effect> EffectPool;
    static std::map<int, Bomb> BombPool;
    static std::map<int, Saw> SawPool;
    static std::map<int, Door> DoorPool;

    CApp();

    void SolveCollisions(int numIterations, double& dt);

    int OnExecute();

    bool OnInit();

    void OnEvent(SDL_Event* Event);

    void SwitchActiveSheep(int newActiveSheep);

    void SelectAllSheepsInCurrentRoom();

    void checkTranslation();

    void OnGesture();

    void OnLoop();

    void OnInitLoop();

    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

    void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

    void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);

    void OnDoubleClick(int mX, int mY);

    void OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value);

    void OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);

    void OnMultitouchEvent();

    void OnCameraContinuousTranslation();

    void OnJoyButtonDown(Uint8 which,Uint8 button);

    void GoToGesture(std::vector<PointDouble>& gesturePositions);
    void GoTo(PointDouble& point);

    void GoTo(std::vector<PointDouble>& controls);

    void OnExit();

    void OnRender();

    void OnCleanup();

    void OthersFollow(int followedSheep);

    void OthersStopFollow();

    void OthersStop();

    void OnGameOver();

    void OnResize(int w, int h);

    void triggerSwitchesOnTouch();

    static void clearScreen();
};

extern CApp theApp;
// lua binding functions
// C++ -> lua
Error CallFunc(lua_State *L, const char *funcname, const Value& arg);
Error CallFunc(lua_State *L, const char *funcname, std::vector<Value>& args);

// lua -> C++
int spawn_entity(lua_State *L);
int entity_goto(lua_State *L);
int entity_goto_next_position(lua_State *L);
int entity_patrol(lua_State *L);

#endif
