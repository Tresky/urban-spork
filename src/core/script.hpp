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

// Singleton object
class ScriptEngine;
extern ScriptEngine* ScriptManager;
extern bool SCRIPT_DEBUG;

/**
 *
 */
class Script
{
public:
  /**
   * Constructor
   */
  Script() {}

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

protected:
  lua_State *L;

  std::string filename;
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
