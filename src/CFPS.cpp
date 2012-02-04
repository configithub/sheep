#include "CFPS.h"

CFPS CFPS::FPSControl;

CFPS::CFPS() {
    OldTime     = 0;
    LastTime    = 0;

    SpeedFactor    = 0;

    Frames      = 0;
    NumFrames   = 0;
}

// framerate monitoring during the main loop
void CFPS::OnLoop() {

    // count the number of frames in the last second
    // 1 frame = one iteration of the main loop
    // i.e. [ event catch + onLoop + render ]
    if(OldTime + 1000 < SDL_GetTicks()) {
        OldTime = SDL_GetTicks();

        // store the number of frame in the last second
        // high for fast systems
        // low for slow systems
        NumFrames = Frames;

        // reset the frame counter to begin couting frames for the next second
        Frames = 0;
    }

    // speed factor determines at which speed we must move an object
    // in order to have consistent speed across various system perfs
    // based on the duration of one frame (getTickets - lastTime)
    // and an input desired movement rate (32 here)
    _frameDuration = SDL_GetTicks() - LastTime;
    SpeedFactor = (_frameDuration / 1000.0f) * 32.0f;
    // speedfactor is invertly proportional to framerate
    // low for fast systems: we move a little number of pixels / frame
    // high for fast systems: we move a big number of pixels / frame
    // in all cases, we move a constant number of pixels / second


    LastTime = SDL_GetTicks();

    Frames++;
}

int CFPS::getFrameDuration() {
  return _frameDuration;
}

int CFPS::GetFPS() {
    return NumFrames;
}

float CFPS::GetSpeedFactor() {
    return SpeedFactor;
}
