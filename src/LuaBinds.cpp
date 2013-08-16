#include "CApp.h"

// C++ -> Lua
//
static void
push_value(lua_State *L, const Value &val)
{
    switch (val.get_type()) {
    case Value::NIL: lua_pushnil(L); break;
    case Value::DOUBLE: lua_pushnumber(L, val.get_double()); break;
    case Value::INT: lua_pushinteger(L, val.get_int()); break;
    case Value::STRING: lua_pushstring(L, val.get_string()); break;
    }
}

Error CallFunc(lua_State *L, const char *funcname, const Value& arg) {
  int retval;
  //std::cout << "calling " << funcname << " with arg " << arg << std::endl;
  lua_getglobal(L, funcname);
  push_value(L, arg);
  retval=lua_pcall(L,1,0,0);
  if (retval!=0) // error
  {
    lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
  }
  return (Error)retval;
}

Error CallFunc(lua_State *L, const char *funcname, std::vector<Value>& args) {
  int retval;
  lua_getglobal(L, funcname);
  for(std::vector<Value>::iterator itValue = args.begin(); 
      itValue != args.end(); ++itValue) { 
    push_value(L, *itValue);
  }  
  retval=lua_pcall(L,args.size(),0,0);
  if (retval!=0) // error
  {
    lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
  }
  return (Error)retval;
}


// Lua -> C++
int spawn_entity(lua_State *L) {
  //std::cout << "entering spawn entity" << std::endl;
  int nargs = lua_gettop(L);
  //std::cout << "number of arguments: " << nargs << std::endl;
  if(nargs % 3 == 0) {
    for (int i = 1; i <= nargs; i+=3) {
      EN_EntityType entityType = (EN_EntityType)lua_tointeger(L, i);
      //std::cout << "entityType: " << entityType << std::endl;
      int entityX = lua_tonumber(L, i+1);
      //std::cout << "entityX: " << entityX << std::endl;
      int entityY = lua_tonumber(L, i+2);
      //std::cout << "entityY: " << entityY << std::endl;
      PointDouble position(entityX, entityY);
      int key = CEntity::CurrentEntityId;
      //std::cout << "key: " << key << std::endl;
      CApp::EntityPool[key].generateAtPos(position, entityType, &theApp);
      //std::cout << "after generateAtPos" << std::endl;
    }

    // return code 0
    lua_pushnumber(L, 0);
  }else{
    // send error code as a result
    lua_pushnumber(L, -1);
  }
  return 1;
}


int entity_goto(lua_State *L) {
  //std::cout << "entering entity goto" << std::endl;
  int nargs = lua_gettop(L);
  //std::cout << "number of arguments: " << nargs << std::endl;
  if(nargs % 3 == 0) {
    for (int i = 1; i <= nargs; i+=3) {
      int entityId = lua_tointeger(L, i);
      //std::cout << "entityId: " << entityId << std::endl;
      int targetX = lua_tonumber(L, i+1);
      //std::cout << "targetX: " << targetX << std::endl;
      int targetY = lua_tonumber(L, i+2);
      //std::cout << "targetY: " << targetY << std::endl;
      PointDouble position(targetX, targetY);
      std::map<int, CEntity>::iterator itFindEntity = CApp::EntityPool.find(entityId);
      if(itFindEntity != CApp::EntityPool.end()) {
        CEntity& entity = itFindEntity->second;
        entity.isTargettingPosition(true);
        entity.getTargetPosition().set(position.getX(), position.getY());
      }
    }
    // return code 0
    lua_pushnumber(L, 0);
  }else{
    // send error code as a result
    lua_pushnumber(L, -1);
  }
  return 1;
}

int entity_goto_next_position(lua_State *L) {
  //std::cout << "entering entity goto next position" << std::endl;
  int nargs = lua_gettop(L);
  //std::cout << "number of arguments: " << nargs << std::endl;
  int entityId = lua_tointeger(L, 1);
  //std::cout << "entityId: " << entityId << std::endl;
  std::map<int, CEntity>::iterator itFindEntity = CApp::EntityPool.find(entityId);
  if(itFindEntity != CApp::EntityPool.end()) {
    CEntity& entity = itFindEntity->second;
    PointDouble& position = entity.b->getTargets()[entity.b->getAttribute(NEXT_POSITION_ID).get_int()];
    //std::cout << "entity.b->getAttribute(NEXT_POSITION_ID): " << entity.b->getAttribute(NEXT_POSITION_ID) << std::endl;
    //std::cout << "entity.b->getTargets().size(): " << entity.b->getTargets().size() << std::endl;
    entity.b->setAttribute(NEXT_POSITION_ID, Value((int) ( (entity.b->getAttribute(NEXT_POSITION_ID).get_int()+1) % entity.b->getTargets().size()) ) );
    entity.isTargettingPosition(true);
    //std::cout << "next position " << position.getX() << " " << position.getY() << std::endl;
    entity.getTargetPosition().set(position.getX(), position.getY());
  }
  // return code 0
  lua_pushnumber(L, 0);
  return 1;
}


int entity_patrol(lua_State *L) {
  //std::cout << "entering entity patrol" << std::endl;
  int nargs = lua_gettop(L);
  //std::cout << "number of arguments: " << nargs << std::endl;
  if( (nargs-1) % 2 == 0) {
    //std::cout << "inside if " << std::endl;
    int entityId = lua_tointeger(L, 1);
    std::map<int, CEntity>::iterator itFindEntity = CApp::EntityPool.find(entityId);
    if(itFindEntity != CApp::EntityPool.end()) {
      CEntity& entity = itFindEntity->second;
      for (int i = 2; i <= nargs; i+=2) {
        //std::cout << "entityId: " << entityId << std::endl;
        int targetX = lua_tonumber(L, i);
        //std::cout << "targetX: " << targetX << std::endl;
        int targetY = lua_tonumber(L, i+1);
        //std::cout << "targetY: " << targetY << std::endl;
        entity.b->getTargets().push_back(PointDouble(targetX, targetY));
      }
      entity.b->setAttribute(STOP_ACTION, Value(CALLBACK));
      entity.b->setAttribute(STOP_TARGET, Value("entity_goto_next_position"));
      entity.b->setAttribute(NEXT_POSITION_ID, Value((int)0));
    }
    // return code 0
    lua_pushnumber(L, 0);
  }else{
    // send error code as a result
    lua_pushnumber(L, -1);
  }
  return 1;
}







//Error CallFunc(lua_State *L, const char *funcname, const ByteVec& arg) {
//int retval;
//lua_getglobal(L, funcname);
//lua_pushlstring (L, &arg[0], arg.size());
//retval=lua_pcall(L,1,0,0);
//if (retval!=0) // error
//{
//	lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
//}
//return _lua_err_code(retval);
//}
