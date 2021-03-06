#include "../utils/globals.hpp"

#include "script_read.hpp"

namespace rpg_script
{

/********************************
 * ReadScript Class Definitions *
 ********************************/
ReadScript::ReadScript()
  : L(ScriptManager->GetGlobalState())
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

  // TPTODO: Need to check the actual filesystem for a real file.

  // These return 0 on success, so the logic is backwards... dumb idea!
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
  if (!IsOpen())
  {
    errors.push(ScriptError { ScriptError::FILE_NOT_OPENED, "No script open. Call OpenFile() first." });
    return false;
  }

  // No tables opened, so we need to open globally
  if (open_tables.empty())
  {
    errors.push(ScriptError { ScriptError::TABLE_NOT_FOUND, "Unable to open integer table globally: " + std::to_string(_key) });
    return false;
  }
  else
  {
    LuaRef temp = open_tables.top()[_key];
    if (temp.isTable())
      open_tables.push(temp);
    else
    {
      errors.push(ScriptError { ScriptError::TABLE_NOT_FOUND, "Table not found in script: " + std::to_string(_key) });
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

// This only calls global functions with no params
void ReadScript::CallFunction(const std::string& _func_name)
{
  if (!IsOpen())
  {
    errors.push(ScriptError { ScriptError::FILE_NOT_OPENED, "No script open. Call OpenFile() first." });
    return;
  }

  // TPTODO: This really isn't very good. Very limited. Improve functionality.
  try {
    getGlobal(L, _func_name.c_str())();
  }
  catch (luabridge::LuaException const& e)
  {
    errors.push(ScriptError { ScriptError::FUNCTION_FAILED, "LuaError: " + std::string(e.what())});
  }
}

} // namespace rpg_script
