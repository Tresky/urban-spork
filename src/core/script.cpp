#include "../utils/globals.hpp"
#include "script.hpp"

#include "../utils/lua_bridge.hpp"
using namespace luabridge;

namespace rpg_script
{

ScriptEngine* ScriptManager = nullptr;
bool SCRIPT_DEBUG = true;

/**********************************
 * ScriptEngine Class Definitions *
 **********************************/
ScriptEngine::ScriptEngine()
  : L(luaL_newstate())
{
  luaL_openlibs(L);
  IF_PRINT_DEBUG(SCRIPT_DEBUG) << "ScriptEngine constructor called" << endl;
}

ScriptEngine::~ScriptEngine()
{
  IF_PRINT_DEBUG(SCRIPT_DEBUG) << "ScriptEngine destructor called" << endl;

  if (L)
    lua_close(L);
}

bool ScriptEngine::InitSingleton()
{
  return true;
}

} //! namespace rpg_script
