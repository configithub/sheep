#include "CApp.h"
#include <iostream>
//#include <math.h>


void CApp::OnRender() {


  clearScreen();
  _background2_x += (int) (FPS::FPSControl.GetSpeedFactor() * _background2_speed);
  _background2_x = _background2_x % WWIDTH;
  Area::Background2.OnRender(-Camera::CameraControl.GetX() - _background2_x,
                         -Camera::CameraControl.GetY());
  _background1_x += (int) (FPS::FPSControl.GetSpeedFactor() * _background1_speed);
  _background1_x = _background1_x % WWIDTH;
  Area::Background1.OnRender(-Camera::CameraControl.GetX() - _background1_x,
                         -Camera::CameraControl.GetY());
  Area::AreaControl.OnRender(-Camera::CameraControl.GetX(), -Camera::CameraControl.GetY());


  //std::cout << "Camera::CameraControl.GetX(): " << Camera::CameraControl.GetX() << std::endl;

  for(int i = 0;i < Entity::EntityList.size();i++) {
    if(!Entity::EntityList[i]) continue;
    Entity::EntityList[i]->OnRender();
  }

  SDL_GL_SwapBuffers();

  FPS::FPSControl.recordCalcDuration();
  //  fix the framerate to avoid using 100% cpu
  if(FPS::FPSControl.getCalcDuration() < 1000/FIXED_FPS) {
    Uint32 delaytime = 1000.0/FIXED_FPS - FPS::FPSControl.getCalcDuration();
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
