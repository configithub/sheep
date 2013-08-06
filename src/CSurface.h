#ifndef _CSURFACE_H_
#define _CSURFACE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <map>

#ifdef ANDROID
#include <GLES/gl.h>
#define glOrtho glOrthof

#include <android/log.h>
#define fprintf(X, ...) __android_log_print(ANDROID_LOG_INFO, "sheepApp", __VA_ARGS__)
#define printf(...) __android_log_print(ANDROID_LOG_INFO, "sheepApp", __VA_ARGS__)
#else
#include <GL/gl.h>
#endif

class CSurface {

  public:

    static int powerOfTwo(int i);

    static bool load(char *name);

    // draw part of Surf_Src to Surf_Dest
    static bool draw(GLuint texture, int X, int Y, int Xtex, int Ytex, int Wtex, int Htex, GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f, GLfloat a = 1.0f);

    static bool OnInit(); // load sprites

    static std::vector<GLuint> Sprites; 
    static std::map<GLuint, std::pair<int,int> > WHmap; 

};

#endif
