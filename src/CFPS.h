#ifndef _CFPS_H_
    #define _CFPS_H_

#include <SDL.h>

// controles game speed for consistency across different system performances
class CFPS {

    private:
        int     OldTime;
        int     LastTime;
        int     _frameDuration;
        int     _calcDuration;
        double     SpeedFactor;


        int     Frames;

    public:
        CFPS();
        static CFPS FPSControl;

        int    NumFrames;

        void    OnLoop();
        void    recordCalcDuration();

        int     GetFPS();

        int     getFrameDuration();
        int     getCalcDuration();
    
        bool    one_second;

        double   GetSpeedFactor();
};

#endif
