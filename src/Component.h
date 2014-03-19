#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class Component {
public:

  Component() : _entity_id(-1), _active(false) {}

  const int& getEntityId() const; 
  void setEntityId(const int& iEntityId);  
  void setEntityId(const int iEntityId);  

  void activate();
  void deactivate();

private:
  int _entity_id;
  bool _active;
};

#endif
