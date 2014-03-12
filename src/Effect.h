#ifndef EFFECT_H
#define EFFECT_H

#include "Behavior.h"

class Effect : public Behavior {

  public:

    Effect() : _id(0), _iterationNb(0), _maxIterationNb(1) {
    }

    void OnRender();

    void OnCleanup();
  
    void OnAnimate();

    const int& getId() const { return _id; }
    void setId(const int& id) { _id = id; }

    const int& getIterationNb() const { return _iterationNb; }
    void setIterationNb(const int& iterationNb) { _iterationNb = iterationNb; }

    const int& getMaxIterationNb() const { return _maxIterationNb; }
    void setMaxIterationNb(const int& maxIterationNb) { _maxIterationNb = maxIterationNb; }

  private:

    int _id;
    int _iterationNb;
    int _maxIterationNb;

};



#endif
