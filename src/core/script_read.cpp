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

  L->script_file(_filename);

  is_opened = true;
  filename = _filename;
  return true;
}

void ReadScript::CloseFile()
{
  is_opened = false;
  filename = "";
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
  (*L)[_func_name]();
}

} // namespace rpg_script
