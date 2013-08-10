#include "CApp.h"

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
