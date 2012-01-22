#ifndef _CSURFACE_H_
    #define _CSURFACE_H_

#include <SDL.h>
#include <SDL_image.h>

class CSurface {
    public:
        CSurface();

    public:

        static SDL_Surface* load_zoomed(char *name, int alpha);

        static SDL_Surface* OnLoadBMP(char* File);
        static SDL_Surface* OnLoad(char* File);

        // draw Surf_Src to Surf_Dest
        static bool OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);

        // draw part of Surf_Src to Surf_Dest
        static bool OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);

        // draw part of Surf_Src to Surf_Dest with a rotation by input angle
        static bool OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H, double angle, double zoom);

        // make input color transparent for this surface
        static bool Transparent(SDL_Surface* Surf_Dest, int R, int G, int B);

};

#endif
