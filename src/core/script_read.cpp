#include "../utils/globals.hpp"
#include "script_read.hpp"

using namespace luabridge;

// Script namespace
namespace rpg_script
{

ReadScript::ReadScript()
  : L(ScriptManager->GetLuaState())
{}

ReadScript::~ReadScript()
{
  CloseFile();
}

bool ReadScript::OpenFile(const std::string& _filename)
{
  if (IsOpen())
  {
    errors.push(ScriptError { ScriptError::FILE_ALREADY_OPEN, "File already opened: " + filename });
    return false;
  }

  // If the _filename is empty
  if (_filename.empty())
  {
    errors.push(ScriptError { ScriptError::FILE_NOT_FOUND, "Script not found: " + _filename });
    return false;
  }

  // TODO: Need to check the actual filesystem for a real file.

  // These return 0 on success, so the logic is backwards
  if (luaL_loadfile(L, _filename.c_str()) || lua_pcall(L, 0, 0, 0))
  {
    errors.push(ScriptError { ScriptError::FILE_NOT_OPENED, "Script failed to open: " + _filename });
    return false;
  }

  is_opened = true;
  filename = _filename;
  return true;
}

void ReadScript::CloseFile()
{
  is_opened = false;
  filename = "";
}

bool ReadScript::OpenTable(const std::string& _table_name)
{
  if (!IsOpen())
  {
    errors.push(ScriptError { ScriptError::FILE_NOT_OPENED, "No script open. Call OpenFile() first." });
    return false;
  }

  // No tables opened, so we need to open globally
  if (open_tables.empty())
  {
    LuaRef temp = getGlobal(L, _table_name.c_str());
    if (temp.isNil())
    {
      errors.push(ScriptError { ScriptError::TABLE_NOT_FOUND, "Table not found in script: " + _table_name });
      return false;
    }
    open_tables.push(temp);
  }
  else
  {
    LuaRef temp = open_tables.top()[_table_name];
    if (temp.isTable())
      open_tables.push(temp);
    else
    {
      errors.push(ScriptError { ScriptError::TABLE_NOT_FOUND, "Table not found in script: " + _table_name });
      return false;
    }
  }
  return true;
}

bool ReadScript::OpenTableIntegers(const int _key)
{
  ScriptError err;

  if (!IsOpen())
  {
    errors.push(ScriptError { ScriptError::FILE_NOT_OPENED, "No script open. Call OpenFile() first." });
    return false;
  }

  // No tables opened, so we need to open globally
  if (open_tables.empty())
  {
    errors.push(ScriptError { ScriptError::TABLE_NOT_FOUND, "Unable to open integer table globally: " + _key });
    return false;
  }
  else
  {
    LuaRef temp = open_tables.top()[_key];
    if (temp.isTable())
      open_tables.push(temp);
    else
    {
      errors.push(ScriptError { ScriptError::TABLE_NOT_FOUND, "Table not found in script: " + _key });
      PRINT_WARNING << "Unable to open table (doesn't exist): " << _key << endl;
      return false;
    }
  }
  return true;
}

void ReadScript::CloseTable()
{
  if (!open_tables.empty())
    open_tables.pop();
}

} // Script namespace
