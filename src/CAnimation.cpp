#include "CAnimation.h"

CAnimation::CAnimation() {
  CurrentFrame    = 0;
  MaxFrames       = 0;
  MinFrames       = 0;
  FrameInc        = 1;

  FrameRate       = 100; //Milliseconds
  OldTime         = 0;

  Oscillate       = false;
  signal = false;
}

void CAnimation::OnAnimate() {
  if(SDL_GetTicks() < OldTime + FrameRate) {
    return;
  }

  OldTime = SDL_GetTicks();

  CurrentFrame += FrameInc;

  if(Oscillate) {
    if(FrameInc > 0) {
      if(CurrentFrame >= MaxFrames - 1) {
        FrameInc = -FrameInc;
      }
    }else{
      if(CurrentFrame <= MinFrames) {
        FrameInc = -FrameInc;
        signal = !signal;
      }
    }
  }else{
    if(CurrentFrame > MaxFrames ) {
      CurrentFrame = MinFrames;
      signal = !signal;
    }
  }
}

void CAnimation::SetFrameRate(int Rate) {
  FrameRate = Rate;
}

void CAnimation::SetCurrentFrame(int Frame) {
  if(Frame < MinFrames || Frame >= MaxFrames) return;

  CurrentFrame = Frame;
}

int CAnimation::GetCurrentFrame() {
  return CurrentFrame;
}
