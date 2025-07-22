#include <core/scripting/scripting.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <core/interface/interface.hpp>
#include <core/renderer/entity/objectEntity.hpp>
#include <core/window/window.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h> 
#include <string>
#include <unordered_map>
#include <vector>

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

QuavleEngine::ObjectEntity entityScript;
QuavleEngine::interface iface;
QuavleEngine::WindowManager windowManagerForScript;

using namespace Scripting;

// Key state tracking for better input handling
static std::unordered_map<int, bool> keyStates;
static std::unordered_map<int, bool> previousKeyStates;

// Update key states (call this every frame)
void updateKeyStates() {
    GLFWwindow* window = windowManagerForScript.getWindow();
    if (!window) return;
    
    // Update previous states
    previousKeyStates = keyStates;
    
    // Common keys to track
    std::vector<int> keysToTrack = {
        GLFW_KEY_SPACE, GLFW_KEY_APOSTROPHE, GLFW_KEY_COMMA, GLFW_KEY_MINUS,
        GLFW_KEY_PERIOD, GLFW_KEY_SLASH, GLFW_KEY_0, GLFW_KEY_1, GLFW_KEY_2,
        GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8,
        GLFW_KEY_9, GLFW_KEY_SEMICOLON, GLFW_KEY_EQUAL, GLFW_KEY_A, GLFW_KEY_B,
        GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H,
        GLFW_KEY_I, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_M, GLFW_KEY_N,
        GLFW_KEY_O, GLFW_KEY_P, GLFW_KEY_Q, GLFW_KEY_R, GLFW_KEY_S, GLFW_KEY_T,
        GLFW_KEY_U, GLFW_KEY_V, GLFW_KEY_W, GLFW_KEY_X, GLFW_KEY_Y, GLFW_KEY_Z,
        GLFW_KEY_LEFT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_RIGHT_BRACKET,
        GLFW_KEY_ESCAPE, GLFW_KEY_ENTER, GLFW_KEY_TAB, GLFW_KEY_BACKSPACE,
        GLFW_KEY_INSERT, GLFW_KEY_DELETE, GLFW_KEY_RIGHT, GLFW_KEY_LEFT,
        GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_PAGE_UP, GLFW_KEY_PAGE_DOWN,
        GLFW_KEY_HOME, GLFW_KEY_END, GLFW_KEY_CAPS_LOCK, GLFW_KEY_SCROLL_LOCK,
        GLFW_KEY_NUM_LOCK, GLFW_KEY_PRINT_SCREEN, GLFW_KEY_PAUSE,
        GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5,
        GLFW_KEY_F6, GLFW_KEY_F7, GLFW_KEY_F8, GLFW_KEY_F9, GLFW_KEY_F10,
        GLFW_KEY_F11, GLFW_KEY_F12, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL,
        GLFW_KEY_LEFT_ALT, GLFW_KEY_LEFT_SUPER, GLFW_KEY_RIGHT_SHIFT,
        GLFW_KEY_RIGHT_CONTROL, GLFW_KEY_RIGHT_ALT, GLFW_KEY_RIGHT_SUPER
    };
    
    // Update key states
    for (int key : keysToTrack) {
        keyStates[key] = (glfwGetKey(window, key) == GLFW_PRESS);
    }
}

// Helper function to find object by UUID
QuavleEngine::ObjectEntity::ObjectData* findObjectByUUID(const std::string& uuid) {
    for (auto& obj : entityScript.objects) {
        if (obj.UUID == uuid) {
            return &obj;
        }
    }
    return nullptr;
}

// Helper function to find object by name
QuavleEngine::ObjectEntity::ObjectData* findObjectByName(const std::string& name) {
    for (auto& obj : entityScript.objects) {
        if (obj.name == name) {
            return &obj;
        }
    }
    return nullptr;
}

// Enhanced debug print with levels
int lua_debugPrint(lua_State* L) {
    int n = lua_gettop(L);
    std::string level = "Info";
    std::string msg;
    
    if (n >= 2) {
        level = luaL_checkstring(L, 1);
        msg = luaL_checkstring(L, 2);
    } else {
        msg = luaL_checkstring(L, 1);
    }
    
    iface.inputDebug(level, msg);
    return 0;
}

// Enhanced position functions
int lua_GetPosition(lua_State* L) {
    const char* id = luaL_checkstring(L, 1);
    auto* obj = findObjectByUUID(id);
    
    if (obj) {
        lua_newtable(L);
        lua_pushnumber(L, obj->position.x);
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, obj->position.y);
        lua_setfield(L, -2, "y");
        lua_pushnumber(L, obj->position.z);
        lua_setfield(L, -2, "z");
        return 1;
    }
    
    lua_pushnil(L);
    return 1;
}

int lua_SetPosition(lua_State* L) {
    const char* id = luaL_checkstring(L, 1);
    
    // Support both table and individual parameters
    if (lua_istable(L, 2)) {
        lua_getfield(L, 2, "x");
        lua_getfield(L, 2, "y");
        lua_getfield(L, 2, "z");
        
        float x = luaL_checknumber(L, -3);
        float y = luaL_checknumber(L, -2);
        float z = luaL_checknumber(L, -1);
        
        auto* obj = findObjectByUUID(id);
        if (obj) {
            obj->position = glm::vec3(x, y, z);
        }
    } else {
        float x = luaL_checknumber(L, 2);
        float y = luaL_checknumber(L, 3);
        float z = luaL_checknumber(L, 4);
        
        auto* obj = findObjectByUUID(id);
        if (obj) {
            obj->position = glm::vec3(x, y, z);
        }
    }
    
    return 0;
}

// Enhanced object management
int lua_GetObjectByName(lua_State* L) {
    const char* name = luaL_checkstring(L, 1);
    auto* obj = findObjectByName(name);
    
    if (obj) {
        lua_pushstring(L, obj->UUID.c_str());
        return 1;
    }
    
    lua_pushnil(L);
    return 1;
}

int lua_ChangeObjectName(lua_State* L) {
    const char* uuid = luaL_checkstring(L, 1);
    const char* newName = luaL_checkstring(L, 2);
    
    auto* obj = findObjectByUUID(uuid);
    if (obj) {
        obj->name = newName;
        lua_pushboolean(L, 1);
    } else {
        lua_pushboolean(L, 0);
    }
    
    return 1;
}

// Get all objects
int lua_GetAllObjects(lua_State* L) {
    lua_newtable(L);
    
    for (size_t i = 0; i < entityScript.objects.size(); i++) {
        lua_newtable(L);
        
        lua_pushstring(L, entityScript.objects[i].UUID.c_str());
        lua_setfield(L, -2, "uuid");
        
        lua_pushstring(L, entityScript.objects[i].name.c_str());
        lua_setfield(L, -2, "name");
        
        // Position
        lua_newtable(L);
        lua_pushnumber(L, entityScript.objects[i].position.x);
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, entityScript.objects[i].position.y);
        lua_setfield(L, -2, "y");
        lua_pushnumber(L, entityScript.objects[i].position.z);
        lua_setfield(L, -2, "z");
        lua_setfield(L, -2, "position");
        
        lua_rawseti(L, -2, i + 1);
    }
    
    return 1;
}

// Enhanced keyboard input functions
int lua_IsKeyPressed(lua_State* L) {
    int key = luaL_checkinteger(L, 1);
    
    GLFWwindow* window = windowManagerForScript.getWindow();
    if (!window) {
        lua_pushboolean(L, 0);
        return 1;
    }
    
    int state = glfwGetKey(window, key);
    lua_pushboolean(L, state == GLFW_PRESS);
    return 1;
}

int lua_IsKeyReleased(lua_State* L) {
    int key = luaL_checkinteger(L, 1);
    
    GLFWwindow* window = windowManagerForScript.getWindow();
    if (!window) {
        lua_pushboolean(L, 0);
        return 1;
    }
    
    int state = glfwGetKey(window, key);
    lua_pushboolean(L, state == GLFW_RELEASE);
    return 1;
}

int lua_IsKeyJustPressed(lua_State* L) {
    int key = luaL_checkinteger(L, 1);
    
    bool currentState = keyStates[key];
    bool previousState = previousKeyStates[key];
    
    lua_pushboolean(L, currentState && !previousState);
    return 1;
}

int lua_IsKeyJustReleased(lua_State* L) {
    int key = luaL_checkinteger(L, 1);
    
    bool currentState = keyStates[key];
    bool previousState = previousKeyStates[key];
    
    lua_pushboolean(L, !currentState && previousState);
    return 1;
}

// Key constants for easier use in Lua
int lua_GetKeyCode(lua_State* L) {
    const char* keyName = luaL_checkstring(L, 1);
    
    // Define key mappings
    static std::unordered_map<std::string, int> keyMap = {
        {"SPACE", GLFW_KEY_SPACE},
        {"ENTER", GLFW_KEY_ENTER},
        {"TAB", GLFW_KEY_TAB},
        {"BACKSPACE", GLFW_KEY_BACKSPACE},
        {"ESCAPE", GLFW_KEY_ESCAPE},
        {"DELETE", GLFW_KEY_DELETE},
        {"UP", GLFW_KEY_UP},
        {"DOWN", GLFW_KEY_DOWN},
        {"LEFT", GLFW_KEY_LEFT},
        {"RIGHT", GLFW_KEY_RIGHT},
        {"SHIFT", GLFW_KEY_LEFT_SHIFT},
        {"CTRL", GLFW_KEY_LEFT_CONTROL},
        {"ALT", GLFW_KEY_LEFT_ALT},
        {"A", GLFW_KEY_A}, {"B", GLFW_KEY_B}, {"C", GLFW_KEY_C},
        {"D", GLFW_KEY_D}, {"E", GLFW_KEY_E}, {"F", GLFW_KEY_F},
        {"G", GLFW_KEY_G}, {"H", GLFW_KEY_H}, {"I", GLFW_KEY_I},
        {"J", GLFW_KEY_J}, {"K", GLFW_KEY_K}, {"L", GLFW_KEY_L},
        {"M", GLFW_KEY_M}, {"N", GLFW_KEY_N}, {"O", GLFW_KEY_O},
        {"P", GLFW_KEY_P}, {"Q", GLFW_KEY_Q}, {"R", GLFW_KEY_R},
        {"S", GLFW_KEY_S}, {"T", GLFW_KEY_T}, {"U", GLFW_KEY_U},
        {"V", GLFW_KEY_V}, {"W", GLFW_KEY_W}, {"X", GLFW_KEY_X},
        {"Y", GLFW_KEY_Y}, {"Z", GLFW_KEY_Z},
        {"0", GLFW_KEY_0}, {"1", GLFW_KEY_1}, {"2", GLFW_KEY_2},
        {"3", GLFW_KEY_3}, {"4", GLFW_KEY_4}, {"5", GLFW_KEY_5},
        {"6", GLFW_KEY_6}, {"7", GLFW_KEY_7}, {"8", GLFW_KEY_8},
        {"9", GLFW_KEY_9},
        {"F1", GLFW_KEY_F1}, {"F2", GLFW_KEY_F2}, {"F3", GLFW_KEY_F3},
        {"F4", GLFW_KEY_F4}, {"F5", GLFW_KEY_F5}, {"F6", GLFW_KEY_F6},
        {"F7", GLFW_KEY_F7}, {"F8", GLFW_KEY_F8}, {"F9", GLFW_KEY_F9},
        {"F10", GLFW_KEY_F10}, {"F11", GLFW_KEY_F11}, {"F12", GLFW_KEY_F12}
    };
    
    auto it = keyMap.find(keyName);
    if (it != keyMap.end()) {
        lua_pushinteger(L, it->second);
    } else {
        lua_pushnil(L);
    }
    
    return 1;
}

// Utility functions
int lua_Sleep(lua_State* L) {
    double seconds = luaL_checknumber(L, 1);
    glfwWaitEventsTimeout(seconds);
    return 0;
}

int lua_GetTime(lua_State* L) {
    lua_pushnumber(L, glfwGetTime());
    return 1;
}

// Vector3 helper functions
int lua_CreateVector3(lua_State* L) {
    float x = luaL_optnumber(L, 1, 0.0f);
    float y = luaL_optnumber(L, 2, 0.0f);
    float z = luaL_optnumber(L, 3, 0.0f);
    
    lua_newtable(L);
    lua_pushnumber(L, x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, z);
    lua_setfield(L, -2, "z");
    
    return 1;
}

int lua_AddVector3(lua_State* L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TTABLE);
    
    lua_getfield(L, 1, "x");
    lua_getfield(L, 1, "y");
    lua_getfield(L, 1, "z");
    float x1 = luaL_checknumber(L, -3);
    float y1 = luaL_checknumber(L, -2);
    float z1 = luaL_checknumber(L, -1);
    lua_pop(L, 3);
    
    lua_getfield(L, 2, "x");
    lua_getfield(L, 2, "y");
    lua_getfield(L, 2, "z");
    float x2 = luaL_checknumber(L, -3);
    float y2 = luaL_checknumber(L, -2);
    float z2 = luaL_checknumber(L, -1);
    lua_pop(L, 3);
    
    lua_newtable(L);
    lua_pushnumber(L, x1 + x2);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, y1 + y2);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, z1 + z2);
    lua_setfield(L, -2, "z");
    
    return 1;
}

void script::scripting() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    
    // Update key states
    updateKeyStates();
    
    // Register enhanced functions
    lua_register(L, "debugPrint", lua_debugPrint);
    lua_register(L, "GetPosition", lua_GetPosition);
    lua_register(L, "SetPosition", lua_SetPosition);
    lua_register(L, "GetObjectByName", lua_GetObjectByName);
    lua_register(L, "ChangeObjectName", lua_ChangeObjectName);
    lua_register(L, "GetAllObjects", lua_GetAllObjects);
    
    // Keyboard input functions
    lua_register(L, "IsKeyPressed", lua_IsKeyPressed);
    lua_register(L, "IsKeyReleased", lua_IsKeyReleased);
    lua_register(L, "IsKeyJustPressed", lua_IsKeyJustPressed);
    lua_register(L, "IsKeyJustReleased", lua_IsKeyJustReleased);
    lua_register(L, "GetKeyCode", lua_GetKeyCode);
    
    // Utility functions
    lua_register(L, "Sleep", lua_Sleep);
    lua_register(L, "GetTime", lua_GetTime);
    
    // Vector3 helper functions
    lua_register(L, "CreateVector3", lua_CreateVector3);
    lua_register(L, "AddVector3", lua_AddVector3);
    
    // Execute all Lua scripts in the specified directory
    std::string scriptDir = "E:/QuavleEngine/build/projects/assets/script/";
    for (const auto& entry : std::filesystem::directory_iterator(scriptDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".lua") {
            std::string scriptPath = entry.path().string();
            if (luaL_dofile(L, scriptPath.c_str()) != LUA_OK) {
                std::cerr << "Lua error in " << scriptPath << ": " << lua_tostring(L, -1) << std::endl;
                lua_pop(L, 1);
            }
        }
    }
    lua_close(L);
}

void script::newScript(std::string scriptName, int index) {
    std::string scriptPath = "projects/assets/script/" + scriptName + ".lua";
    if (!std::filesystem::exists(scriptPath)) {
        std::ofstream ofs(scriptPath);
        entityScript.objects[index].scriptLocation.push_back(scriptPath);
        iface.isCodeEditor = true;
        iface.loadCode(scriptPath);
        return;
    }else{
        iface.isCodeEditor = true;
        iface.loadCode(scriptPath);
        entityScript.objects[index].scriptLocation.push_back(scriptPath);
    }
}


// Call this function every frame to update input states
void script::updateInput() {
    updateKeyStates();
}