#include "../utils/globals.hpp"
#include "script_read.hpp"

using namespace luabridge;

// Script namespace
namespace rpg_script
{

ReadScript::ReadScript()
  : L(ScriptManager->GetLuaState())
  , current_ref(L)
{ luaL_openlibs(L);
}

ReadScript::~ReadScript()
{
  CloseFile();
}

bool ReadScript::OpenFile(const std::string& _filename)
{
  cout << "Opening: " << _filename << endl;
  // These return 0 on success, so the logic is backwards
  if (luaL_loadfile(L, _filename.c_str()) || lua_pcall(L, 0, 0, 0))
  {
    PRINT_ERROR << "Script failed to load: " << _filename << endl;
    return false;
  }

  filename = _filename;
  return true;
}

void ReadScript::CloseFile()
{
  // if (L)
  //   lua_close(L);

  current_ref = Nil();
}

bool ReadScript::OpenTable(const std::string& _table_name)
{
  if (!L)
  {
    PRINT_ERROR << "No Lua script open. Call OpenFile() first." << endl;
    return false;
  }

  current_ref = getGlobal(L, _table_name.c_str());
  if (current_ref.isNil())
  {
    PRINT_WARNING << "Unable to open table: " << _table_name << endl;
    return false;
  }
  return true;
}

} // Script namespace
