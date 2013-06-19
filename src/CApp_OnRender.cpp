#include "CApp.h"
#include <iostream>
//#include <SDL_rotozoom.h>
//#include <math.h>

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


  Area::AreaControl.OnRender(Surf_Display, -CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());

  for(int i = 0;i < CEntity::EntityList.size();i++) {
    if(!CEntity::EntityList[i]) continue;
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


  // TODO score display here
  //CSurface::OnDraw(Surf_Display, Surf_Score, 5, 5,1 ,1 ,44,9);
  //print_num(Surf_Display, Surf_NumFont, 50, 4, score);


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

