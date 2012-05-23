#ifndef GRASSPATCH_H
#define GRASSPATCH_H

#include "Vectorial.h"
#include "CArea.h"
#include "Define.h"
#include <SDL.h>

class GrassPatch {


  public:

    GrassPatch() : _isValidated(false) {}

    int& getNbSheepsForValidation() { return _nbSheepsForValidation; }
    int& getNbSheepsValidated() { return _nbValidated; }
    void setNbSheepsValidated(int nbSheeps) { _nbValidated = nbSheeps; }
    void setNbSheepsForValidation(int& iNbSheeps) { _nbSheepsForValidation = iNbSheeps; }
    void setNbSheepsForValidation(int iNbSheeps) { _nbSheepsForValidation = iNbSheeps; }
    Rectangle& getPatch() { return _patch; }



    void generateRandom(Rectangle& boundaries, int minWidth, int minHeight, int maxWidth, int maxHeight) {


      /* initialize random seed: */
      //srand ( SDL_GetTicks());

      /* generate grass patch: */
      _patch.setWidth( rand() % maxWidth + minWidth );
      _patch.setHeight( rand() % maxHeight + minHeight );
      _patch.getCorner().setX( rand() % (boundaries.getWidth() - _patch.getWidth()) + TILE_SIZE + 1 );
      _patch.getCorner().setY( rand() % (boundaries.getHeight() - _patch.getHeight()) + TILE_SIZE + 1 );


    }

    bool& isValidated() { return _isValidated; }

    void reset() { _nbValidated = 0; _isValidated = false; }

  private:

    int _nbValidated;
    int _nbSheepsForValidation;
    bool _isValidated;
    Rectangle _patch;

};




#endif
