#ifndef SCRIPT_READ_HPP
#define SCRIPT_READ_HPP

#include "script.hpp"

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

  //!
  luabridge::LuaRef current_ref;
};

template <class T> T ReadScript::ReadData(const std::string& _key, T _default)
{
  luabridge::LuaRef ref = (current_ref)[_key];
  if (ref.isNil())
    return _default;

  return ref.cast<T>();
}

template <class T> T ReadScript::ReadData(const int _key, T _default)
{
  luabridge::LuaRef ref = (current_ref)[_key];
  if (ref.isNil())
    return _default;

  return ref.cast<T>();
}

}

#endif // SCRIPT_READ_HPP
