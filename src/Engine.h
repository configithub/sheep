#ifndef _ENGINE_H_
#define _ENGINE_H_

#include<vector>

class Entity;
class Component;
class System;

class Engine {
public:
  const int& createEntity();
  void addSystem(System& iSystem);
private:
  std::vector<Entity> _entities;   
  std::map<int, Component*> _components_added;
  std::map<int, Component*> _components_removed;

  std::vector<System> _systems;
};

#endif
