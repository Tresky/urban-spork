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
   * Opens a table in the Lua script that has a string
   * type name denoting it. This is opposed to an integer
   * named table. For opening a table with an integer name
   * please see OpenTableIntegers().
   *
   * This function will check to see if the table being
   * requested if available at the current place in the table
   * tree. If it is, it will open it from there; otherwise,
   * the function will attempt to open the table from the
   * global scope. If that also fails, an error will be generated.
   *
   * \see OpenTableIntegers()
   *
   * \param _table_name String containing the name of the table to open
   * \return True if successful, false otherwise

   */
  bool OpenTable(const std::string& _table_name);

  /**
   * Opens a table in the Lua script that has an integer
   * type name denoting it. This is opposed to a string
   * named table. For opening a table with a string name
   * please see OpenTable()
   *
   * This function will check to see if the table being
   * requested if available at the current place in the table
   * tree. If it is, it will open it from there; otherwise,
   * the function will attempt to open the table from the
   * global scope. If that also fails, an error will be generated.
   *
   * \see OpenTable()
   *
   * \param  _index Integer value denoting the table to open
   * \return True if successful, false otherwise
   */
  bool OpenTableIntegers(const int _index);

  /**
   * Closes the currently open table. All opened tables
   * are stored in a FILO stack. Calling this function
   * will pop the top-most table from the stack, allowing
   * you to access elements in the previous level.
   */
  void CloseTable();

  /**
   * Templated function to read data from the Lua script.
   * There are two versions of this function; this version
   * is specialized to read data from properties with <b>string</b>
   * type identifiers.
   * \param  _key     String for key to read
   * \param  _default Default error value; will be returned if _key isn't found
   * \return Value found at the [_key], _default if error
   */
  template <class T> T ReadData(const std::string& _key, T _default);

  /**
   * Templated function to read data from the Lua script.
   * There are two versions of this function; this version
   * is specialized to read data from properties with <b>integer</b>
   * type identifiers.
   * \param  _key     Integer for key to read
   * \param  _default Default error value; will be returned if _key isn't found
   * \return Value found at the [_key], _default if error
   */
  template <class T> T ReadData(const int _key, T _default);

  /**
   * Calls a function from the global scope of the current
   * Lua script that is open.
   * \param _func_name String containing the Lua function to execute
   */
  void CallFunction(const std::string& _func_name);

private:
  //! Pointer to Lua State; copied from ScriptEngine
  lua_State* L;

  //! Stack containing references to all of the open tables
  std::stack<luabridge::LuaRef> open_tables;
}; // class ReadScript

template <class T> T ReadScript::ReadData(const std::string& _key, T _default)
{
  luabridge::LuaRef ref = open_tables.top()[_key];
  if (ref.isNil())
  {
    errors.push(ScriptError { ScriptError::DATA_NOT_FOUND, "Data not found in table: " + _key });
    return _default;
  }

  return ref.cast<T>();
}

template <class T> T ReadScript::ReadData(const int _key, T _default)
{
  luabridge::LuaRef ref = open_tables.top()[_key];
  if (ref.isNil())
  {
    errors.push(ScriptError { ScriptError::DATA_NOT_FOUND, "Data not found in table: " + std::to_string(_key) });
    return _default;
  }

  return ref.cast<T>();
}

} // namespace rpg_script

#endif // SCRIPT_READ_HPP
