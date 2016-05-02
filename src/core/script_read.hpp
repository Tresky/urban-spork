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
  virtual bool OpenFile(std::string& _filename);

  /**
   * Closes the currently opened file.
   */
  virtual void CloseFile();

  /**
   * Templated function to read data from the Lua script.
   * @param  _key     String for key to read
   * @param  _default Default error value
   * @return          Value found at the [_key], _default if error
   */
  template <class T> T ReadData(const std::string& _key, T _default);
  template <class T> T ReadData(const int _key, T _default);

private:
  // Pointer to Lua State
  lua_State* L;
};

template <class T> T ReadScript::ReadData(const std::string& _key, T _default)
{
  LuaRef ref = getGlobal(L, _key);
  if (ref.isNil())
  {
    IF_PRINT_DEBUG(SCRIPT_DEBUG) << "Unable to get variable: " << _key << endl;
    return _default;
  }

  return ref.cast<T>;
}

template <class T> T ReadScript::ReadData(const int _key, T _default)
{
  LuaRef ref = getGlobal(L, _key);
  if (ref.isNil())
  {
    IF_PRINT_DEBUG(SCRIPT_DEBUG) << "Unable to get variable: " << _key << endl;
    return _default;
  }

  return ref.cast<T>;
}

}

#endif // SCRIPT_READ_HPP
