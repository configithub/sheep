#ifndef GRASSPATCH_H
#define GRASSPATCH_H

#include "Vectorial.h"
#include "CArea.h"
#include "Define.h"

class GrassPatch {


  public:

    GrassPatch() : _isValidated(false) {}

    int& getNbSheepsForValidation() { return _nbSheepsForValidation; }
    int& getNbSheepsValidated() { return _validated; }
    void setNbSheepsForValidation(int& iNbSheeps) { _nbSheepsForValidation = iNbSheeps; }
    void setNbSheepsForValidation(int iNbSheeps) { _nbSheepsForValidation = iNbSheeps; }

    std::vector<int>& getTiles() { return _tiles; }

    generateRandom(Rectangle& boundaries, int& minWidth, int& minHeight, int& maxWidth, int& maxHeight) {

      Rectangle grassPatch;

      /* initialize random seed: */
      srand ( time(NULL) );

      /* generate grass patch: */
      grassPatch.setWidht( rand() % maxWidth + minWidth );
      grassPatch.setHeight( rand() % maxHeight + minHeight );
      grassPatch.getCorner().setX( rand() % (boundaries.getWidth() - grassPatch.getWidth()) );
      grassPatch.getCorner().setY( rand() % (boundaries.getHeight() - grassPatch.getHeight()) );

      // retrieve tile ids from map
      // factor in collision offsets
      int StartX = grassPatch.getCorner().getX() / TILE_SIZE;
      int StartY = grassPatch.getCorner().getY() / TILE_SIZE;

      int EndX = ( grassPatch.getCorner().getX() + grassPatch.getWidth() ) / TILE_SIZE;
      int EndY = ( grassPatch.getCorner().getY() + grassPatch.getHeight() )  / TILE_SIZE;

      // determine which tiles our entity is over
      for(int iY = StartY; iY <= EndY; iY++) {
        for(int iX = StartX; iX <= EndX; iX++) {
          CTile* Tile = CArea::AreaControl.GetTile(iX * TILE_SIZE, iY * TILE_SIZE);
          _tiles.push_back(Tile->TileID);
        }
      }


    }

    bool& isValidated() { return _isValidated; }

  private:

    int _nbValidated;
    int _nbSheepsForValidation;
    std::vector<int> _tiles;
    bool _isValidated;


};




#endif
