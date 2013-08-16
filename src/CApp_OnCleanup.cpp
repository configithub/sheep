#include "CApp.h"

void CApp::OnCleanup() {
  // clean SDL objects
  //SDL_FreeSurface(Surf_Display);

  // clean all entities
  /*for(int i = 0;i < CEntity::EntityList.size();i++) {
    if(!CEntity::EntityList[i]) continue;

    CEntity::EntityList[i]->OnCleanup();
    }

    CEntity::EntityList.clear();*/

  Area::AreaControl.OnCleanup();
  // cleanup Lua 
  lua_close(luaState);

  // and terminate SDL
  SDL_Quit();
}

