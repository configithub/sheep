#ifndef _CENTITY_H_
    #define _CENTITY_H_

#include <vector>
#include <map>
#include <set>

#include "CArea.h"
#include "CAnimation.h"
#include "CCamera.h"

#include "CSurface.h"

enum {
    ENTITY_TYPE_GENERIC = 0,

    ENTITY_TYPE_PLAYER
};

enum {
    ENTITY_FLAG_NONE     = 0,

    ENTITY_FLAG_GRAVITY    = 0x00000001,
    ENTITY_FLAG_GHOST    = 0x00000002,
    ENTITY_FLAG_MAPONLY    = 0x00000004
};


class CEntityCol;

class CEntity {

    friend class CFollower;
    public:
        static std::vector<CEntity*>    EntityList;
        static int CurrentEntityId;
        float           gotoX;
        float           gotoY;

        CEntity();

        virtual ~CEntity();

        virtual bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        virtual void OnLoop();

        void OnLoopApplyGravity();

        void OnLoopApplyControls();

        void OnLoopDeriveAndCapSpeed(float& dt);

        void OnLoopMotionBounds(float& dt);

        void OnLoopRealizeMotion(float& dt);

        //void ApplyMap(float& dt);

        virtual void OnSpeculateMove(float velX, float velY, float dt , bool enableJump=false);


        virtual void OnRender(SDL_Surface* Surf_Display);

        virtual void OnCleanup();

        virtual void OnAnimate();

        virtual bool OnCollision(CEntity* Entity);

        virtual bool OnCollision(CEntity* Entity, CEntityCol* EntityCol, int nbCollisionCheck, int maxCollisionCheck);




        void    StopMove();

        void    StopMove(CEntity* Support);


        void    updateX(double& NewX);

        bool    Collides(int oX, int oY, int oW, int oH, CEntityCol& ioEntityCol);

        bool    SpeculativeCollides(int oX, int oY, int oW, int oH, CEntityCol& ioEntityCol);

        bool    Jump();

        bool    Stop();

        bool   PosValidOnEntities(int NewX, int NewY);


    protected:
        virtual bool     PosValid(int NewX, int NewY);

        virtual bool     PosValidOnMap(int NewX, int NewY);



        bool     PosValidTile(CTile* Tile);

        bool     PosValidEntity(CEntity* Entity, int NewX, int NewY);


    protected:
        bool    CanJump;



        CAnimation      Anim_Control;

        SDL_Surface*    Surf_Entity;



        float        AccelX;
        float        AccelY;

        int             CurrentFrameCol;
        int             CurrentFrameRow;




    public:
       int        Col_X;
        int        Col_Y;
        int        Col_Width;
        int        Col_Height;


        float        SpeedX;
        float        SpeedY;

        bool    CanFollow;

        int entityId;

        float           X;
        float           Y;

        bool GotoCommand;

        int             Width;
        int             Height;

        // calculated in OnLoopMotionBounds
        float NextX;
        float NextY;

        bool        MoveLeft;
        bool        MoveRight;

        bool        MoveUp;
        bool        MoveDown;

        int        Type;

        bool        Dead;
        int         Flags;

        float        MaxSpeedX;
        float        MaxSpeedY;

        CEntity* NearestEntity;


};

class CEntityCol {
    public:
        static std::map<std::pair<int, int>, CEntityCol>    EntityColList;

        // rectangle of the intersection of bounding boxes for this collision
        //float           X;
        //float           Y;
        int             Width;
        int             Height;

    public:
        CEntity* EntityA;

        CEntity* EntityB;

    public:
        CEntityCol();
        void updateContactSize();
};

#endif
