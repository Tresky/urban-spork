/**
 * \file script_read.hpp
 *
 * Contains a child-class of Script that is
 * specialized for reading input from a Lua script.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 06-18-16
 */

#ifndef SCRIPT_READ_HPP
#define SCRIPT_READ_HPP

#include "script.hpp"

#include <stack>

namespace rpg_script
{

/**
 * \class ReadScript
 * \brief Child-class of Script that is specialized
 * for reading input and executing Lua scripts.
 *
 * \ingroup ScriptEngine
 *
 * This class serves a single purpose: to read and
 * execute Lua code from a given file. An instance
 * of this class will be able to open a Lua file using
 * the global Lua state from ScriptEngine and provides
 * public members to be able to navigate through a
 * script, execute code, as well as read data.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 06-18-16
 */
class ReadScript: public Script
{
  friend class ScriptEngine;
public:
  /**
   * Default constructor
   */
  ReadScript();

  /**
   * Destructor
   */
  virtual ~ReadScript();

  /**
   * Opens a lua script from the given filename.
   * \param  _filename String for the filename to the Lua script
   * \return True if successful, false otherwise
   */
  virtual bool OpenFile(const std::string& _filename);

  /**
   * Closes the currently opened file. This function
   * doesn't do much except for set a flag that
   * denotes that the file has been closed.
   */
  virtual void CloseFile();

  /**
   * Templated function that opens a Lua table with a name
   * that is either a string or integer.
   *
   * This function will check to see if the table being
   * requested if available at the current place in the table
   * tree. If it is, it will open it from there; otherwise,
   * the function will attempt to open the table from the
   * global scope. If that also fails, an error will be generated.
   *
   * \param _table_name String or int containing the name of the table to open
   * \return True if successful, false otherwise

   */
  template <class T> bool OpenTable(const T& _table_name);

  /**
   * Closes the currently open table. All opened tables
   * are stored in a FILO stack. Calling this function
   * will pop the top-most table from the stack, allowing
   * you to access elements in the previous level.
   */
  void CloseTable();

  /**
   * Templated functions to read data from the Lua script. This
   * function can read values from tables with <b>string</b> or
   * <b>integer</b> key names.
   * \param  _key     String or int for key to read
   * \param  _default Default error value; will be returned if _key isn't found
   * \return Value found at the [_key], _default if error
   */
  template <class T> T ReadData(const std::string& _key, T _default);
  template <class T> T ReadData(const int& _key, T _default);

  /**
   * Calls a function from the global scope of the current
   * Lua script that is open.
   * \param _func_name String containing the Lua function to execute
   */
  void CallFunction(const std::string& _func_name);

private:
  //! Pointer to Lua State; copied from ScriptEngine
  sol::state* L;

  //! Stack containing references to all of the open tables
  std::stack<sol::table> open_tables;
}; // class ReadScript

template <class T> bool ReadScript::OpenTable(const T& _table_name)
{
  if (!IsOpen())
  {
    errors.push(ScriptError { ScriptError::FILE_NOT_OPENED, "No script open. Call OpenFile() first." });
    return false;
  }

  // No tables opened, so we need to open globally
  sol::table temp;
  if (open_tables.empty())
    temp = (*L)[_table_name];
  // Otherwise open from the most recently opened table
  else
    temp = open_tables.top()[_table_name];

  // if (temp.get_type() == sol::nil)
  // {
  //   errors.push(ScriptError { ScriptError::TABLE_NOT_FOUND, "Table not found in script: " + _table_name });
  //   return false;
  // }
  open_tables.push(temp);

  return true;
}

template <class T> T ReadScript::ReadData(const std::string& _path, T _default)
{
  // Make sure the file is open before reading data
  if (!IsOpen())
  {
    errors.push(ScriptError { ScriptError::FILE_NOT_OPENED, "No script open. Call OpenFile() first." });
    return _default;
  }

  // No tables opened, so we need to read globally
  T temp;
  if (open_tables.empty())
    temp = (*L)[_path].get_or<T>(_default);
  // Otherwise open from the most recently opened table
  else
    temp = open_tables.top()[_path].get_or<T>(_default);

  if (temp == _default)
  {
    errors.push(ScriptError { ScriptError::TABLE_NOT_FOUND, "Table not found in script: " + _path });
    return _default;
  }

  return temp;
}

template <class T> T ReadScript::ReadData(const int& _path, T _default)
{
  // Make sure the file is open before reading data
  if (!IsOpen())
  {
    errors.push(ScriptError { ScriptError::FILE_NOT_OPENED, "No script open. Call OpenFile() first." });
    return _default;
  }

  // No tables opened, so we need to read globally
  T temp;
  if (open_tables.empty())
    temp = (*L)[_path].get_or<T>(_default);
  // Otherwise open from the most recently opened table
  else
    temp = open_tables.top()[_path].get_or<T>(_default);

  if (temp == _default)
  {
    errors.push(ScriptError { ScriptError::TABLE_NOT_FOUND, "Table not found in script: " + std::to_string(_path) });
    return _default;
  }

  return temp;
}

} // namespace rpg_script

#endif // SCRIPT_READ_HPP
