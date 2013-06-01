#include "CSurface.h"
#include <iostream>
//#include <SDL_rotozoom.h>
#include <assert.h> 

std::vector<SDL_Surface*> CSurface::Sprites;

CSurface::CSurface() {
}



/*----------------------------------------------------------
  General surface tool functions
  ----------------------------------------------------------*/
/*----------------------------------------------------------
  General tool functions
  ----------------------------------------------------------*/



/*
 * Load and convert an antialiazed, zoomed set of sprites.
 */
SDL_Surface * CSurface::load_zoomed(char *name, int alpha)
{
  SDL_Surface *sprites;
  SDL_Surface *temp = IMG_Load(name);
  if(!temp)
    return NULL;

  if(!temp)
  {
    fprintf(stderr, "Could not clean alpha!\n");
    return NULL;
  }

  /*if(alpha)
    {
    SDL_SetAlpha(temp, 0, SDL_ALPHA_OPAQUE);
    sprites = SDL_DisplayFormatAlpha(temp);
    }
    else
    {*/
  SDL_SetColorKey(temp, SDL_SRCCOLORKEY,
      SDL_MapRGB(temp->format, 255, 0, 255));
  sprites = SDL_DisplayFormat(temp);
  //}
  SDL_FreeSurface(temp);
  std::cout << "bit per pixel: " <<  (int)sprites->format->BitsPerPixel << std::endl;
  //assert(sprites->format->BitsPerPixel == 8);
  return sprites;
}


bool CSurface::OnInit() {

  if(!Sprites.empty()) { return false; }

  // sheep
  SDL_Surface* sheepSurface = NULL;
  if( (sheepSurface = CSurface::OnLoad("./gfx/sheep6.png")) == NULL ) { return false; }
  Sprites.push_back(sheepSurface);

  // bomb
  SDL_Surface* bombSurface = NULL;
  if( (bombSurface = CSurface::OnLoad("./gfx/bomb.png")) == NULL ) { return false; }
  Sprites.push_back(bombSurface);

  // switch
  SDL_Surface* switchSurface = NULL;
  if( (switchSurface = CSurface::OnLoad("./gfx/switch.png")) == NULL ) { return false; }
  Sprites.push_back(switchSurface);

  // explosion
  SDL_Surface* explosionSurface = NULL;
  if( (explosionSurface = CSurface::OnLoad("./gfx/explosion.png")) == NULL ) { return false; }
  Sprites.push_back(explosionSurface);

  // saw
  SDL_Surface* sawSurface = NULL;
  if( (sawSurface = CSurface::OnLoad("./gfx/saw2.png")) == NULL ) { return false; }
  Sprites.push_back(sawSurface);

  return true;
}


// creates a displayable surface from a bmp image file
SDL_Surface* CSurface::OnLoadBMP(char* File) {
  //SDL_Surface* Surf_Temp = NULL;
  SDL_Surface* Surf_Return = NULL;

  // try to load a temporary surface with a bmp image
  //if((Surf_Return = IMG_Load(File)) == NULL) {
  if((Surf_Return = SDL_LoadBMP(File)) == NULL) {
    std::cout << "surface load from image file failed" << std::endl;
    return NULL;
  }

  // copies temporary surface to another one to be returned
  // also do some conversion to permit the display within the current SDL settings
  //Surf_Return = SDL_DisplayFormatAlpha(Surf_Temp);
  // alpha for alpha blending => transparent png look transparent as a surface
  //SDL_FreeSurface(Surf_Temp); // deallocate the temporary surface

  return Surf_Return;
}





// creates a displayable surface from a bmp image file
SDL_Surface* CSurface::OnLoad(char* File) {
  //SDL_Surface* Surf_Temp = NULL;
  SDL_Surface* Surf_Return = NULL;
  std::cout << "surface load " << File << std::endl;
  // try to load a temporary surface with a bmp image
  //if((Surf_Return = IMG_Load(File)) == NULL) {
  if((Surf_Return = load_zoomed(File, 0)) == NULL) {
    std::cout << "surface load from image file failed " << File << std::endl;
    return NULL;
  }

  // copies temporary surface to another one to be returned
  // also do some conversion to permit the display within the current SDL settings
  //Surf_Return = SDL_DisplayFormatAlpha(Surf_Temp);
  // alpha for alpha blending => transparent png look transparent as a surface
  //SDL_FreeSurface(Surf_Temp); // deallocate the temporary surface

  return Surf_Return;
}

// draw one source surface on another destination one
bool CSurface::OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y) {
  if(Surf_Dest == NULL || Surf_Src == NULL) {
    return false;
  }

  // rectangle object
  SDL_Rect DestR;

  DestR.x = X;
  DestR.y = Y;
  // we could have precised the height and weidth also (w and h)

  // draw source on destination according to coordinate rectangle
  SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);
  // NULL stands for another SDL_Rect type parameter to pick a subpart of the destination surface only

  return true;
}

// draws a rectangular part of the source surface to the destination surface
bool CSurface::OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H) {
  if(Surf_Dest == NULL || Surf_Src == NULL) {
    std::cout << "trying to blit from/to null surface" << std::endl;
    std::cout << "Surf_Dest: " << Surf_Dest << std::endl;
    std::cout << "Surf_Src: " << Surf_Src << std::endl;
    return false;
  }

  SDL_Rect DestR;

  DestR.x = X;
  DestR.y = Y;

  SDL_Rect SrcR;

  SrcR.x = X2;
  SrcR.y = Y2;
  SrcR.w = W;
  SrcR.h = H;



  SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);

  return true;
}

// draws a rectangular part of the source surface to the destination surface
bool CSurface::OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H, double angle, double zoom) {
  if(Surf_Dest == NULL || Surf_Src == NULL) {
    return false;
  }

  SDL_Rect DestR;

  DestR.x = X;
  DestR.y = Y;

  SDL_Rect SrcR;

  SrcR.x = X2;
  SrcR.y = Y2;
  SrcR.w = W;
  SrcR.h = H;

  //rotozoom test
  //SDL_Surface *temp = rotozoomSurface (Surf_Src, angle, zoom, 0/*smooth*/);

  //if(temp == NULL) return false;

  SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);

  //SDL_FreeSurface(temp);

  return true;
}



bool CSurface::Transparent(SDL_Surface* Surf_Dest, int R, int G, int B) {
  if(Surf_Dest == NULL) {
    return false;
  }

  //SDL_SetColorKey(Surf_Dest, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Surf_Dest->format, R, G, B));

  return true;
}

