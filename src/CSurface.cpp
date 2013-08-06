#include "CSurface.h"
#include <iostream>
//#include <SDL_rotozoom.h>
#include <assert.h> 

std::vector<GLuint> CSurface::Sprites;
std::map<GLuint, std::pair<int,int> > CSurface::WHmap; 


int CSurface::powerOfTwo(int i) {
  int r;
  for (r = 1; r < i; r *= 2) {}
  return r;
}

SDL_Color translate_color(Uint32 int_color)                           
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  SDL_Color color={static_cast<unsigned char>(int_color & 0x00ff0000)/0x10000, static_cast<unsigned char>(int_color &
      0x0000ff00)/0x100, static_cast<unsigned char>(int_color & 0x000000ff),0};
#else
  SDL_Color color={ static_cast<unsigned char>(int_color & 0x000000ff), static_cast<unsigned char>(int_color &
      0x0000ff00)/0x100, static_cast<unsigned char>(int_color & 0x00ff0000)/0x10000,0};    
#endif
  return color;
}

bool CSurface::load(char *name) {
  SDL_Surface *pic = IMG_Load(name);
  /*if(pic != NULL) {
    std::cout << pic->format->BitsPerPixel << std::endl;
    }else{
    std::cout << "pic surface null" << std::endl;
    }
    if(pic == NULL ||
    (pic->format->BitsPerPixel != 32
    || pic->format->BitsPerPixel != 16
    || pic->format->BitsPerPixel != 24) ) {
    fprintf(stderr, "Could not load pic\n");
    SDL_FreeSurface(pic);
    return NULL;
    }*/

  //SDL_SetColorKey(pic, SDL_SRCCOLORKEY,
  //    SDL_MapRGB(pic->format, 255, 0, 255));

  // color keying in SDL before creating the opengl texture
  SDL_Color color;
  for (int y = 0; y < pic->h; y++) {
    for (int x = 0; x < pic->w; x++) {
      unsigned int pix = ((unsigned int*)pic->pixels)[y*(pic->pitch/sizeof(unsigned int)) + x];
      color =  translate_color(pix);
      if (color.r == 255 && color.b == 255)    
      {((unsigned int*)pic->pixels)[y*(pic->pitch/sizeof(unsigned int)) + x] = SDL_MapRGBA(pic->format, 0, 0, 0,0);}
    }
  }

  GLenum glFormat = (pic->format->BitsPerPixel == 32 ? GL_RGBA : GL_RGB);
  int w = pic->w; int h = pic->h;
  int upload_w = powerOfTwo(w);
  int upload_h = powerOfTwo(h);
  float texcoord_w = (float) w / (float) upload_w;
  float texcoord_h = (float) h / (float) upload_h;

  std::cout << "name: " << name << std::endl;
  std::cout << "pic->format->BitsPerPixel: " << pic->format->BitsPerPixel << std::endl;
  
  

  GLuint currentTexture = 0;
  glGenTextures(1, &currentTexture);

  glBindTexture(GL_TEXTURE_2D, currentTexture);
  std::cout << "currentTexture: " << currentTexture << std::endl;
  Sprites.push_back(currentTexture);
  std::cout << "Sprites.back(): " << Sprites.back() << std::endl;

  WHmap[currentTexture] = std::make_pair(upload_w, upload_h);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, glFormat, upload_w, upload_h, 0, glFormat, GL_UNSIGNED_BYTE, NULL);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, glFormat, GL_UNSIGNED_BYTE, pic->pixels);

  std::cout << "bit per pixel: " <<  (int)pic->format->BitsPerPixel << std::endl;
  SDL_FreeSurface(pic);

  std::cout << "glGetError(): " << glGetError() << std::endl;

  return true;
}


bool CSurface::OnInit() {

  if(!Sprites.empty()) { return false; }

  // sheep
  if( !CSurface::load("./gfx/sheep.png") ) { std::cout << "fail load sheep.png" << std::endl; return false; }
  // bomb
  if( !CSurface::load("./gfx/bomb.png") ) { return false; }
  // switch
  if( !CSurface::load("./gfx/switch.png") ) { return false; }
  // explosion
  if( !CSurface::load("./gfx/explosion.png") ) { return false; }
  // saw
  if( !CSurface::load("./gfx/saw.png") ) { return false; }
  // door
  if( !CSurface::load("./gfx/door.png") ) { return false; }

  std::cout << "entity sprites initalized" << std::endl;  

  return true;
}




bool CSurface::draw(GLuint texture, int X, int Y, int Xtex, int Ytex, int Wtex, int Htex, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {

  int& totalTexW = WHmap[texture].first;
  int& totalTexH = WHmap[texture].second;
  float X1 = Xtex / (float)totalTexW;  
  //std::cout << "X1: " << X1 << std::endl;
  float Y1 = Ytex / (float)totalTexH;  
  //std::cout << "Y1: " << Y1 << std::endl;

  float X2 = (Xtex + Wtex) / (float)totalTexW;  
  //std::cout << "X2: " << X2 << std::endl;

  float Y2 = (Ytex + Htex) / (float)totalTexH;  
  //std::cout << "Y2: " << Y2 << std::endl;

  //std::cout << "X: " << X << std::endl;
  //std::cout << "Y: " << Y << std::endl;
  //std::cout << "Wtex: " << Wtex << std::endl;
  //std::cout << "Htex: " << Htex << std::endl;

  GLfloat textureCoordinates[] = { X1, Y1, // top left
    X2, Y1,  // top right
    X2, Y2,  // bottom right
    X1, Y2}; // bottom left
  GLfloat vertices[] = { (float)X, (float)Y, // top left
    (float)X + Wtex, (float)Y, // top right
    (float)X + Wtex, (float)Y + Htex, // bottom right
    (float)X, (float)Y + Htex }; // bottom left
  glColor4f(r, g, b, a);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // You may just replace all your GL_QUADS with GL_TRIANGLE_FAN and it will draw absolutely identically with same coordinates, if you have just 4 coords.
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  return true;
}


