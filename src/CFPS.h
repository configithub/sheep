#ifndef _CFPS_H_
    #define _CFPS_H_

#include <SDL.h>

// controles game speed for consistency across different system performances
class CFPS {
    public:
        static CFPS FPSControl;

        int    NumFrames;

    private:
        int     OldTime;
        int     LastTime;
        int     _frameDuration;
        float     SpeedFactor;


        int     Frames;

    public:
        CFPS();

        void    OnLoop();

    public:
        int     GetFPS();

        int     getFrameDuration();

        float   GetSpeedFactor();
};

#endif
