#include "CApp.h"
#include <iostream>
//#include <SDL_rotozoom.h>
#include "Level.h" 

void CApp::print_num(SDL_Surface *dst, SDL_Surface *font, int x, int y, float value)
{
  char buf[16];
  int val = (int)(value * 10.0);
  int pos, p = 0;
  SDL_Rect from;

  /* Sign */
  if(val < 0)
  {
    buf[p++] = 10;
    val = -val;
  }

  /* Integer part */
  pos = 10000000;
  while(pos > 1)
  {
    int num = val / pos;
    val -= num * pos;
    pos /= 10;
    if(p || num)
      buf[p++] = num;
  }

  /* Decimals */
  if(val / pos)
  {
    buf[p++] = 11;
    while(pos > 0)
    {
      int num = val / pos;
      val -= num * pos;
      pos /= 10;
      buf[p++] = num;
    }
  }

  /* Render! */
  from.y = 0;
  from.w = 7;
  from.h = 10;
  for(pos = 0; pos < p; ++pos)
  {
    SDL_Rect to;
    to.x = x + pos * 7;
    to.y = y;
    from.x = buf[pos] * 7;
    SDL_BlitSurface(font, &from, dst, &to);
  }
}


void CApp::OnRender() {

  // draw test surface on main surface at coord 0,0
  //CSurface::OnDraw(Surf_Display, Surf_Test, 0, 0);

  // draw the current animation frame for our animation
  //CSurface::OnDraw(Surf_Display, Surf_Test, 290, 220, 0, Anim_Yoshi.GetCurrentFrame() * 64, 64, 64);

  // draw part (rectangle 0,0,25,25) of the test surface on the main surface at coord 100,100
  //CSurface::OnDraw(Surf_Display, Surf_Test, 100, 100, 0, 0, 25, 25);

  // refresh main surface, once everything is in place in memory
  // AKA double buffering (enabled at SDL init time)

  //CArea::AreaControl.OnRender(Surf_Display, CCamera::CameraControl.GetX(), CCamera::CameraControl.GetY());

  Level::LevelInstance.OnRender();

  CArea::AreaControl.OnRender(Surf_Display, -CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());

  for(int i = 0;i < CEntity::EntityList.size();i++) {
    if(!CEntity::EntityList[i]) continue;
    //std::cout << CEntity::EntityList[i]->getEntityId() << std::endl;
    CEntity::EntityList[i]->OnRender(Surf_Display);
  }

  //std::cout << "framerate " << CFPS::FPSControl.NumFrames << std::endl;
  //print_num(Surf_Display, Surf_NumFont, Surf_Display->w-117, 12, CFPS::FPSControl.NumFrames);

  //print_num(Surf_Display, Surf_NumFont, 160, 2, Mouse.getX());
  //print_num(Surf_Display, Surf_NumFont, 200, 2, Mouse.getY());

  for(Uint8 i =0; i < MultitouchEvent::Controller.getNumberOfTouches(); ++i) {

    if(MultitouchEvent::Controller.isActivePoint(i)) {
      //print_num(Surf_Display, Surf_NumFont, 160, 20+18*i, MultitouchEvent::Controller.getTouch(i).getX() );
      //print_num(Surf_Display, Surf_NumFont, 200, 20+18*i, MultitouchEvent::Controller.getTouch(i).getY() );
    }else{
      //print_num(Surf_Display, Surf_NumFont, 160, 20+18*i, 0 );
      //print_num(Surf_Display, Surf_NumFont, 200, 20+18*i, 0 );
    }

  }

  if(Level::LevelInstance.isGameOver()) {
    OnGameOver();
  }

  //print_num(Surf_Display, Surf_NumFont, 240, 11, JoyAxis.getY());


  //print_num(Surf_Display, Surf_NumFont, Surf_Display->w-117, 12, 1);
  //print_num(Surf_Display, Surf_NumFont, 100, 100, 1);

  //rotozoom test
  //Surf_Display = SDL_DisplayFormat(rotozoomSurface (Surf_Display, 180, 1, 0/*smooth*/));

  //if(Surf_Display == NULL) return ;

  SDL_Flip(Surf_Display);

  // fix the framerate to avoid using 100% cpu
  if(CFPS::FPSControl.getFrameDuration() < 1000/FIXED_FPS) {
    Uint32 delaytime = 1000/FIXED_FPS - CFPS::FPSControl.getFrameDuration();
    SDL_Delay((Uint32)delaytime);
  }

}

