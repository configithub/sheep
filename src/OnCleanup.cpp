#include "CApp.h"

void CApp::OnCleanup() {
  // clean SDL objects
  //SDL_FreeSurface(Surf_Display);

  // clean all entities
  /*for(int i = 0;i < Entity::EntityList.size();i++) {
    if(!Entity::EntityList[i]) continue;

    Entity::EntityList[i]->OnCleanup();
    }

    Entity::EntityList.clear();*/

  Area::AreaControl.OnCleanup();
  Area::Background1.OnCleanup();
  Area::Background2.OnCleanup();
  // cleanup Lua 
  lua_close(luaState);

  // and terminate SDL
  SDL_Quit();
}

