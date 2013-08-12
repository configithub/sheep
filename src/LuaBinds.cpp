#include "CApp.h"

// C++ -> Lua
//
static void
push_value(lua_State *L, const Value &val)
{
    switch (val.get_type()) {
    case Value::NIL: lua_pushnil(L); break;
    case Value::DOUBLE: lua_pushnumber(L, to_double(val)); break;
    case Value::INT: lua_pushinteger(L, to_int(val)); break;
    case Value::STRING: lua_pushstring(L, to_string(val)); break;
    }
}

Error CallFunc(lua_State *L, const char *funcname, const Value& arg) {
    int retval;
    lua_getglobal(L, funcname);
    push_value(L, arg);
    retval=lua_pcall(L,2,0,0);
    if (retval!=0) // error
      {
	lua_setglobal (L, "_LASTERROR") ; //Set _LASTERROR to returned error message
      }
    return (Error)retval;
}


// Lua -> C++
int spawn_entity(lua_State *L) {
  std::cout << "entering spawn entity" << std::endl;
  int nargs = lua_gettop(L);
  std::cout << "number of arguments: " << nargs << std::endl;
  if(nargs % 3 == 0) {
    for (int i = 1; i <= nargs; i+=3) {
      EN_EntityType entityType = (EN_EntityType)lua_tointeger(L, i);
      std::cout << "entityType: " << entityType << std::endl;
      int entityX = lua_tonumber(L, i+1);
      std::cout << "entityX: " << entityX << std::endl;
      int entityY = lua_tonumber(L, i+2);
      std::cout << "entityY: " << entityY << std::endl;
      PointDouble position(entityX, entityY);
      int key = CEntity::CurrentEntityId;
      std::cout << "key: " << key << std::endl;
      CApp::EntityPool[key].generateAtPos(position, entityType, &theApp);
      std::cout << "after generateAtPos" << std::endl;
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
