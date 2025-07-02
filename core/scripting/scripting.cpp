#include <core/scripting/scripting.h>
#include <iostream>
#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/window/window.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h> 
#include <string>

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

QuavleEngine::ObjectEntity entityScript;
QuavleEngine::interface iface;

using namespace Scripting;

QuavleEngine::WindowManager windowManagerForScript;


int lua_debugPrint(lua_State* L){
    const char* msg = luaL_checkstring(L, 1);
    iface.inputDebug("Info", std::string(msg));
    return 0;
}

int lua_GetPosition(lua_State* L) {
    const char* id = luaL_checkstring(L, 1); // Ambil argumen pertama sebagai string

    bool found = false;
    glm::vec3 locationObject;

    for (int k = 0; k < entityScript.objects.size(); k++) {
        if (entityScript.objects[k].UUID == id) {
            locationObject = entityScript.objects[k].position;
            found = true;
            break;
        }
    }

    if (found) {
        lua_newtable(L);

        lua_pushnumber(L, locationObject.x);
        lua_setfield(L, -2, "x");

        lua_pushnumber(L, locationObject.y);
        lua_setfield(L, -2, "y");

        lua_pushnumber(L, locationObject.z);
        lua_setfield(L, -2, "z");

        return 1; // Mengembalikan table
    }

    // Kalau tidak ditemukan, return nil
    lua_pushnil(L);
    return 1;
}

int lua_SetPosition(lua_State *L)
{
    const char *id = luaL_checkstring(L, 1);
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    float z = luaL_checknumber(L, 4);

    for (int k = 0; k < entityScript.objects.size(); k++)
    {
        if (entityScript.objects[k].UUID == id)
        {
            entityScript.objects[k].position = glm::vec3(x, y, z);
            break;
        }
    }

    return 0; // Tidak mengembalikan apa pun ke Lua
}


//! MASIH BUG
int lua_getTick(lua_State* L){
    const int tick = glfwGetTime();
    lua_pushnumber(L, tick); // Placeholder: return 0 for now
}
//! ============

//! MASIH BUG
int lua_getMousePos(lua_State* L) {
    double xPos = windowManagerForScript.mousePosX;
    double yPos = windowManagerForScript.mousePosY;

    lua_pushnumber(L, xPos); // return value 1
    lua_pushnumber(L, yPos); // return value 2

    return 2; // Total 2 nilai dikembalikan ke Lua
}
//! ============

void script::scripting()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "GetPosition", lua_GetPosition);
    lua_register(L, "debugPrint", lua_debugPrint);
    lua_register(L, "SetPosition", lua_SetPosition);
    lua_register(L, "GetMousePos", lua_getMousePos);
    lua_register(L, "getTick", lua_getTick);


    if (luaL_dofile(L, "D:/QuavleEngine/build/test/test.lua") != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    lua_close(L);
}
