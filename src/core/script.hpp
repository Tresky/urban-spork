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
  SCRIPT_TYPE_TILEMAP = 1,
  SCRIPT_TYPE_TILESET = 2,
};

// Singleton object
class ScriptEngine;
extern ScriptEngine* ScriptManager;
extern bool SCRIPT_DEBUG;

// class Script
// {
// public:
//   Script(const std::string& _filename);
//
//   ~Script();
//
//   void PrintError(const std::string& _var_name, const std::string& _reasons);
//
//
//
//   template<typename T>
//   T Get(const std::string& _var_name);
//
// private:
//   /**
//    * Various function to return 'null' values
//    * for a variety of different templated types.
//    */
//   // template<>
//   // inline std::string GetDefault()
//   // { return "null"; }
//   //
//   // template<>
//   // inline int GetDefault()
//   // { return 0; }
//   //
//   // template<>
//   // inline float GetDefault()
//   // { return 0.0f; }
//
//   /**
//    * Loads a global table onto the stack and loads variables
//    * from the table.
//    * @param  _var_name String with the variable to load
//    * @return           True if successful, false otherwise
//    */
//   bool GetToStack(const string& _var_name);
//
//   /**
//    * Various functions to
//    */
//   // template<>
//   // inline std::string LuaGet(const std::string& _var_name)
//   // {
//   //   std::string s = "null";
//   //   if (lua_isstring(state, -1))
//   //     s = std::string(lua_tostring(state, -1));
//   //   else
//   //     PrintError(_var_name, "Not a string");
//   //   return s;
//   // }
//   //
//   // template<>
//   // inline int LuaGet(const std::string& _var_name)
//   // {
//   //   if (!lua_isnumber(L, -1))
//   //     PrintError(_var_name, "Not a number");
//   //   return (int)lua_tonumber(state, -1);
//   // }
//   //
//   // template<>
//   // inline float LuaGet(const std::string& _var_name)
//   // {
//   //   if (!lua_isnumber(L, -1))
//   //     PrintError(_var_name, "Not a number");
//   //   return (float)lua_tonumber(state, -1);
//   // }
//   //
//   // template<>
//   // inline bool LuaGet(const std::string& _var_name)
//   // {
//   //     return (bool)lua_toboolean(state, -1);
//   // }
//
//
//   lua_State* state;
//   std::string filename;
// };

/**
 *
 */
class ScriptEngine : public rpg_utils::Singleton<ScriptEngine>
{
  friend class rpg_utils::Singleton<ScriptEngine>;
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
   * Load a Lua script into the engine based on type.
   * @param  _lua_filepath Filepath to the Lua script to load
   * @param  _type         ScriptType to load; decides method of loading
   * @return               True if successful, false otherwise
   */
  // bool Load(std::string& _lua_filepath, ScriptType _type);

  /**
   * Loads the data pertaining to a global character from a Lua script.
   * @param  _lua_filepath Filepath to the Lua script to load
   * @return               True if successful, false otherwise
   */
  bool LoadGlobalCharacter(std::string& _lua_filepath);

  /**
   * Loads the data pertaining to a tilemap from a Lua script.
   * @param  _lua_filepath Filepath to the Lua script to load
   * @param _map           Pointer to the MapMode object to receive data
   * @return               True if successful, false otherwise
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
   */
  bool LoadTileset(const std::string& _lua_filepath, rpg_map_mode::private_map_mode::TileSupervisor* _ts);

  // Lua state object
  lua_State* L;

  // Hash-map that maps all of the Lua scripts to a key
  //std::map<std::string, Script*> scripts;
};

}

#endif // SCRIPT_HPP
