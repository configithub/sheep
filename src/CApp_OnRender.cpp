#include "CApp.h"
#include <iostream>
//#include <math.h>


void CApp::OnRender() {


  clearScreen();
  Area::AreaControl.OnRender(-CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());

  //std::cout << "CCamera::CameraControl.GetX(): " << CCamera::CameraControl.GetX() << std::endl;

  for(int i = 0;i < CEntity::EntityList.size();i++) {
    if(!CEntity::EntityList[i]) continue;
    CEntity::EntityList[i]->OnRender();
  }

  SDL_GL_SwapBuffers();

  CFPS::FPSControl.recordCalcDuration();
  //  fix the framerate to avoid using 100% cpu
  if(CFPS::FPSControl.getCalcDuration() < 1000/FIXED_FPS) {
    Uint32 delaytime = 1000.0/FIXED_FPS - CFPS::FPSControl.getCalcDuration();
    SDL_Delay((Uint32)delaytime);
    // std::cout << "real_delay: " << real_delay << std::endl;
  }

}

void CApp::clearScreen() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Always clear your scene before rendering, unless you're sure that you'll fill whole screen with textures/models etc

  // You have to do this each frame, because SDL messes up with your GL context when drawing on-screen keyboard, however is saves/restores your matrices
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
