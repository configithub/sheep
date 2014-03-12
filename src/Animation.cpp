#include "Animation.h"

Animation::Animation() {
  CurrentFrame    = 0;
  MaxFrames       = 0;
  MinFrames       = 0;
  FrameInc        = 1;

  FrameRate       = 100; //Milliseconds
  OldTime         = 0;

  Oscillate       = false;
  signal = false;
}

void Animation::OnAnimate() {
  if(MaxFrames == MinFrames) { CurrentFrame = MinFrames; return; }
  if(SDL_GetTicks() < OldTime + FrameRate) {
    return;
  }

  OldTime = SDL_GetTicks();

  CurrentFrame += FrameInc;

  if(Oscillate) {
    if(FrameInc > 0) {
      if(CurrentFrame >= MaxFrames ) {
        FrameInc = -FrameInc;
        signal = !signal;
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

void Animation::SetFrameRate(int Rate) {
  FrameRate = Rate;
}

void Animation::SetCurrentFrame(int Frame) {
  if(Frame < MinFrames || Frame >= MaxFrames) return;

  CurrentFrame = Frame;
}

int Animation::GetCurrentFrame() {
  return CurrentFrame;
}
