#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "../utils/singleton.hpp"

// Lua Resources
extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}
#include "../utils/lua_bridge.hpp"

namespace rpg_map_mode
{
class MapMode;
namespace private_map_mode
{ class TileSupervisor; }
}

namespace rpg_script
{

// The various types of script that can be loaded
// from Lua files. These are used to decide how to
// load the script in; which private method to call.
enum ScriptTypes
{
  SCRIPT_TYPE_GLOBAL_CHARACTER = 0,
  SCRIPT_TYPE_ANIMATION = 1,
  SCRIPT_TYPE_TILEMAP = 2,
  SCRIPT_TYPE_TILESET = 3
};

struct ScriptError
{
  enum ErrorType
  {
    NO_ERROR = -1,
    FILE_NOT_FOUND = 0,
    FILE_NOT_OPENED = 1,
    FILE_ALREADY_OPEN = 2,
    DATA_NOT_FOUND = 3,
    DATA_NOT_RETRIEVED = 4,
    TABLE_NOT_FOUND = 5
  } type;

  std::string message;
};

// Singleton object
class ScriptEngine;
extern ScriptEngine* ScriptManager;
extern bool SCRIPT_DEBUG;

/**
 * This class acts as an abstract base class for ReadScript
 * and WriteScript. This allows the ScriptManager to store
 * all of the Script handlers in the same data structure.
 *
 * As this class is abstract, it cannot be instantiated. Any
 * attempt to will generate a compiler error.
 */
class Script
{
public:
  /**
   * Constructor
   */
  Script()
    : is_opened(false)
  {}

  /**
   * Destructor
   */
  virtual ~Script() {}

  /**
   * Virtual function to open a file.
   * @param  _filename String with filename to open
   * @return           True if successful, false otherwise
   */
  virtual bool OpenFile(const std::string& _filename) = 0;

  /**
   * Closes the currently open file.
   */
  virtual void CloseFile() = 0;

  bool IsOpen() const
  {
    return is_opened;
  }

  const std::string GetFilename()
  {
    return filename;
  }

  bool HasError() const
  {
    if (!errors.empty())
      return true;
    return false;
  }

  ScriptError GetNextError()
  {
    ScriptError error = errors.front();
    errors.pop();
    return error;
  }

  void PrintErrors()
  {
    while (!errors.empty())
    {
      ScriptError err = GetNextError();
      PRINT_WARNING << "ScriptError[" << err.type << "]: " << err.message << endl;
    }
  }

protected:
  lua_State *L;

  std::string filename;

  bool is_opened;

  std::queue<ScriptError> errors;
};

/**
 *
 */
class ScriptEngine : public rpg_utils::Singleton<ScriptEngine>
{
  friend class rpg_utils::Singleton<ScriptEngine>;
  friend class Script;
public:
  /**
   * Destructor
   */
  ~ScriptEngine();

  lua_State* GetLuaState()
  {
    return L;
  }

  /**
   * [InitSingleton description]
   * @return [description]
   */
  bool InitSingleton();

  /**
   * Loads the data pertaining to a global character from a Lua script.
   * @param  _lua_filepath Filepath to the Lua script to load
   * @return               True if successful, false otherwise
   *
   * / NOTE: Flagged for Deletion
   */
  bool LoadGlobalCharacter(std::string& _lua_filepath);

  /**
   * Loads the data pertaining to a tilemap from a Lua script.
   * @param  _lua_filepath Filepath to the Lua script to load
   * @param _map           Pointer to the MapMode object to receive data
   * @return               True if successful, false otherwise
   *
   * / NOTE: Flagged for Deletion
   */
  bool LoadTilemap(std::string& _lua_filepath, rpg_map_mode::MapMode* _map);

private:
  /**
   * Constructor only called once from Singleton.
   */
  ScriptEngine();

  /**
   * Loads the data pertaining to a tileset from a Lua script.
   * @param  _lua_filepath Filepath to the Lua script to load
   * @return               True if successful, false otherwise
   *
   * / NOTE: Flagged for Deletion
   */
  bool LoadTileset(const std::string& _lua_filepath, rpg_map_mode::private_map_mode::TileSupervisor* _ts);

  // Lua state object
  lua_State* L;

  // Hash-map that maps all of the open Lua scripts to a key
  std::map<std::string, Script*> open_scripts;
};

}

#endif // SCRIPT_HPP
