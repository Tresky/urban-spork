#include "../utils/globals.hpp"
#include "script.hpp"

#include "../utils/sol2.hpp"

namespace rpg_script
{

ScriptEngine* ScriptManager = nullptr;
bool SCRIPT_DEBUG = true;

/**********************************
 * ScriptEngine Class Definitions *
 **********************************/
ScriptEngine::ScriptEngine()
  : L(new sol::state())
{
  L->open_libraries(sol::lib::base, sol::lib::package);
  IF_PRINT_DEBUG(SCRIPT_DEBUG) << "ScriptEngine constructor called" << endl;
}

ScriptEngine::~ScriptEngine()
{
  IF_PRINT_DEBUG(SCRIPT_DEBUG) << "ScriptEngine destructor called" << endl;
}

bool ScriptEngine::InitSingleton()
{
  return true;
}

} //! namespace rpg_script
