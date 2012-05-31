#ifndef _CANIMATION_H_
    #define _CANIMATION_H_

#include <SDL.h>

class CAnimation {
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

    public:
        CAnimation();

        void OnAnimate();

    public:
        void SetFrameRate(int Rate);

        int GetFrameRate() { return FrameRate; }

        void SetCurrentFrame(int Frame);

        int GetCurrentFrame();
};

#endif
