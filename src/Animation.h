#ifndef _CANIMATION_H_
    #define _CANIMATION_H_

#include <SDL.h>

class Animation {
    private:
        int    CurrentFrame;

        int     FrameInc;

    private:
        int     FrameRate; //Milliseconds

        long    OldTime;

    public:
        int    MaxFrames;
        int    MinFrames;

        bool    Oscillate;

        bool  signal;
    public:
        Animation();

        void OnAnimate();

    public:
        void SetFrameRate(int Rate);

        void SetFrameInc(int Inc) { FrameInc = Inc; }

        int GetFrameRate() { return FrameRate; }

        void SetCurrentFrame(int Frame);

        int GetCurrentFrame();
};

#endif
