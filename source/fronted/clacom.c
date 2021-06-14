#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

int main() {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaL_dofile(L, "scripts/modulo.lua");
  lua_setglobal(L, "mymodule");
//  lua_settop(L, 0);

 int top = lua_gettop(L);

 lua_getglobal(L, "mymodule");
 if (!lua_istable(L, -1)) {
	printf("mymodule isnotable\n");
	return 0;
  }

  lua_getfield(L, -1, "test");  // -1 means stack top.
  lua_call(L, 0, 1);
  if (lua_isinteger(L, -1))  /* integer? */
         printf("resultado %lld\n", lua_tointeger(L, -1));


  lua_close(L);
  return 0;
}
