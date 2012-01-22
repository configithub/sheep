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

class CApp : public CEvent {
    private:
        bool            Running;

        SDL_Surface*    Surf_Display;

        SDL_Surface*    Surf_Test;

        SDL_Surface*    Surf_NumFont;

        int activeSheep;

        bool others_follow;

        CFollower Follower;
        CFollower Follower1;
        CFollower Follower2;
        CFollower Follower3;

        std::vector<CFollower> SheepPool;
        std::vector<CFollower*> Sheeps;

    public:
        CApp();

        void Solve(int numIterations, float& dt);

        int OnExecute();

        bool OnInit();

        bool AddNewSheepInPool(int sheepId);

        void OnEvent(SDL_Event* Event);

        void SwitchActiveSheep(int newActiveSheep);

        void PushArrowUp();

        void PushArrowLeftRight(bool right, bool down);

        void OnLoop();

        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

        void OnLButtonDown(int mX, int mY);

        void OnLButtonUp(int mX, int mY);

	void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);

        void GoTo(int x, int y);

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
