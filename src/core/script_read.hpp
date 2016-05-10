#ifndef SCRIPT_READ_HPP
#define SCRIPT_READ_HPP

#include "script.hpp"

#include <stack>

namespace rpg_script
{

/**
 *
 */
class ReadScript: public Script
{
  friend class ScriptEngine;
public:
  /**
   * Constructor
   */
  ReadScript();

  /**
   * Destructor
   */
  virtual ~ReadScript();

  /**
   * Opens a lua script from the given filename.
   * @param  _filename String for the filepath to the Lua script
   * @return           True if successful, false otherwise
   */
  virtual bool OpenFile(const std::string& _filename);

  /**
   * Closes the currently opened file.
   */
  virtual void CloseFile();

  /**
   * [OpenTable description]
   * @param _table_name [description]
   */
  bool OpenTable(const std::string& _table_name);
  bool OpenTableIntegers(const int _index);

  void CloseTable();

  /**
   * Templated function to read data from the Lua script.
   * @param  _key     String for key to read
   * @param  _default Default error value
   * @return          Value found at the [_key], _default if error
   */
  template <class T> T ReadData(const std::string& _key, T _default);
  template <class T> T ReadData(const int _key, T _default);

private:
  //! Pointer to Lua State
  lua_State* L;

  //! Stack containing references to all of the open tables
  std::stack<luabridge::LuaRef> open_tables;
};

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

}

#endif // SCRIPT_READ_HPP
