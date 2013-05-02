#ifndef _CAPP_H_
#define _CAPP_H_

#include <SDL.h>
#include "CSurface.h"
#include "CEvent.h"
#include "CAnimation.h"
#include "CEntity.h"
#include "Define.h"
#include "CArea.h"
#include "CCamera.h"
#include "CFollower.h"
#include "Vectorial.h"
#include "MultitouchEvent.h"
#include "CFPS.h"
#include "Effect.h"
#include "Bomb.h"
#include "Switch.h"


class CApp : public CEvent {
  private:
    bool            Running;

    SDL_Surface*    Surf_Display;

    SDL_Surface*    Surf_Test;


    //Multitouch     _multitouch;

    std::vector<SDL_Joystick*> _sdlJoysticks;

    int activeSheep;

    bool others_follow;

    //CFollower Follower;



    PointDouble Mouse;

  public:

    //static PointDouble& getMouse() { return Mouse; }

    static int score;
    static int scoreTimer;
    static void updateScore();

    PointDouble* _center;
    PointDouble* _nextCenter;
  
    static std::vector<CEntity*> Sheeps;
    static void print_num(SDL_Surface *dst, SDL_Surface *font, int x, int y, float value);

    // all entities
    static std::map<int, CEntity> EntityPool;

    // behavior by type
    static std::map<int, CFollower> SheepPool;
    static std::map<int, Switch> SwitchPool;
    static std::map<int, Effect> EffectPool;
    static std::map<int, Bomb> BombPool;

    static SDL_Surface*    Surf_NumFont;
    static SDL_Surface*    Surf_NumFontBomb;
    static SDL_Surface*    Surf_GameOver;
    static SDL_Surface*    Surf_Score;
    CApp();

    void SolveCollisions(int numIterations, double& dt);

    int OnExecute();

    bool OnInit();

    void OnEvent(SDL_Event* Event);

    void SwitchActiveSheep(int newActiveSheep);

    void DeselectAllSheeps();

    void SelectAllSheeps();

    void SelectAllSheepsInCurrentRoom();

    void OnLoop();

    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

    void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

    void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);

    void OnDoubleClick(int mX, int mY);

    void OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value);

    void OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);

    void OnMultitouchEvent();

    void OnJoyButtonDown(Uint8 which,Uint8 button);

    void getActiveGroup(std::vector<CEntity*>& ioGroup);

    void SelectHerdAtCoord(PointDouble& point);

    void GetNearestEntityFromMouse(PointDouble& point, CEntity*& NearestEntityFromMouse, int& delta);

    void GoTo(PointDouble& point);

    void GoTo(std::vector<PointDouble>& controls);

    void StopGoTo();

    void OnExit();

    void OnRender();

    void OnCleanup();

    void OthersFollow(int followedSheep);

    void OthersStopFollow();

    void OthersStop();

    void OnGameOver();

    void triggerSwitchesOnTouch();
};

#endif
