#include "Behavior.h"


Behavior::Behavior() {}

// destroy 
Behavior::~Behavior() {}

// decide whether this entity will be part of the next loop or not
void Behavior::OnLoop(){} 

// animate the sprite of the entity
void Behavior::OnAnimate(){}

// render the entity on the display surface
void Behavior::OnRender(){}

// operations to be done at clean up time : free memory
void Behavior::OnCleanup(){}

void Behavior::OnTriggeredAction(int id){}

void Behavior::kill(){}

void Behavior::OnInit(){}

void Behavior::setActionId(int id) {}
void Behavior::setTriggerId(int id) {}
void Behavior::setSwitchId(int id) {}
void Behavior::addTarget(int iTarget) {}
