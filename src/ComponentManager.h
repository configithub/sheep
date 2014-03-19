#ifndef _COMPONENT_MANAGER_H_
#define _COMPONENT_MANAGER_H_

#include <vector>
#include "Component.h"

template <class T> class ComponentManager {
public:
  ComponentManager();
  const int& add(T& iComponent);
  void remove(const int index);
private:
  std::vector<T> _table;
  std::vector<int> _recycle_rows;
};

template <class T> const int& ComponentManager<T>::add(T& iComponent) {
  if(_recycle_rows.empty()) {
    _table.push_back(iComponent);
    _table.back().activate();
  }else{
    int& last_index = _recycle_rows.back();
    _table[last_index] = iComponent;
    _table[last_index].activate();
    _recycle_rows.pop_back();
  }
}

template <class T> void ComponentManager<T>::remove(const int index) {
  _table[index].inactivate();
  _recycle_rows.push_back(index);
}


#endif
