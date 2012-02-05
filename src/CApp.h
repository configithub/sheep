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

class CApp : public CEvent {
    private:
        bool            Running;

        SDL_Surface*    Surf_Display;

        SDL_Surface*    Surf_Test;

        SDL_Surface*    Surf_NumFont;

        //Multitouch     _multitouch;

        std::vector<SDL_Joystick*> _sdlJoysticks;

        int activeSheep;

        bool others_follow;

        CFollower Follower;
        CFollower Follower1;
        CFollower Follower2;
        CFollower Follower3;

        std::vector<CFollower> SheepPool;
        std::vector<CFollower*> Sheeps;

        Point Mouse;
        Point JoyAxis;

    public:
        CApp();

        void Solve(int numIterations, float& dt);

        int OnExecute();

        bool OnInit();

        bool AddNewSheepInPool(int sheepIdbool, double X=300, double Y=300);

        void OnEvent(SDL_Event* Event);

        void SwitchActiveSheep(int newActiveSheep);

        void DeselectAllSheeps();

        void PushArrowUp();

        void PushArrowLeftRight(bool right, bool down);

        void OnLoop();

        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

        void OnLButtonDown(int mX, int mY);

        void OnLButtonUp(int mX, int mY);

	    void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);

	    void OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value);

	    void OnMultitouchEvent();

	    //void OnJoyHat(Uint8 which,Uint8 axis,Sint16 value);

	    //void OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);

	    void OnJoyButtonDown(Uint8 which,Uint8 button);


        void OnJoyButtonUp(Uint8 which,Uint8 button);

        void SelectHerdAtCoord(int x, int y);

        void GetNearestEntityFromMouse(int x, int y, CFollower*& NearestEntityFromMouse, int& delta);

        void GoTo(int x, int y);

        void GoTo(int x, int y, int subgroupNb, int subGroup);

        void GoToIsometric(int x, int y);

        void GoToHybrid(int x, int y);

        void StopGoTo(int x, int y);

        void OnExit();

        void OnRender();

        void OnCleanup();

        void OthersFollow(int followedSheep);

        void OthersStopFollow();

        void OthersStop();

        bool CheckEveryEntityCollision();
};

#endif
