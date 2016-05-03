#include "../utils/globals.hpp"
#include "script_read.hpp"

using namespace luabridge;

// Script namespace
namespace rpg_script
{

ReadScript::ReadScript()
  : L(ScriptManager->GetLuaState())
  , current_ref(L)
  , bookmark(L)
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

void ReadScript::BookmarkCurrentTable()
{
  bookmark = current_ref;
}

void ReadScript::LoadBookmark()
{
  current_ref = bookmark;
}

bool ReadScript::OpenTable(const std::string& _table_name)
{
  if (!L)
  {
    PRINT_ERROR << "No Lua script open. Call OpenFile() first." << endl;
    return false;
  }

  if (current_ref.isNil())
  {
    current_ref = getGlobal(L, _table_name.c_str());
    if (current_ref.isNil())
    {
      PRINT_WARNING << "Unable to open table: " << _table_name << endl;
      return false;
    }
  }
  else
  {
    LuaRef temp = current_ref[_table_name];
    if (temp.isTable())
      current_ref = temp;
    else
    {
      current_ref = getGlobal(L, _table_name.c_str());
      if (current_ref.isNil())
      {
        PRINT_WARNING << "Unable to open table: " << _table_name << endl;
        return false;
      }
    }
  }
  return true;
}

bool ReadScript::OpenTableIntegers(const int _key)
{
  if (!L)
  {
    PRINT_ERROR << "No Lua script open. Call OpenFile() first." << endl;
    return false;
  }

  if (current_ref.isNil())
  {
    PRINT_WARNING << "Unable to open table (no base table open): " << _key << endl;
    return false;
  }
  else
  {
    LuaRef temp = current_ref[_key];
    cout << temp.type() << endl;
    if (temp.isNil())
    {
      PRINT_WARNING << "Unable to open table (doesn't exist): " << _key << endl;
      return false;
    }
    current_ref = temp;
  }
  return true;
}

} // Script namespace
