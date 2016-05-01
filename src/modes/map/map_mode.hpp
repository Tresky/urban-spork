#ifndef MAP_MODE_HPP
#define MAP_MODE_HPP

#include "../../core/mode_manager.hpp"
#include "map_utils.hpp"

namespace rpg_script
{ class ScriptEngine; }

namespace rpg_map_mode
{

namespace private_map_mode
{ class TileSupervisor;
  class ObjectSupervisor;
  class MapFrame; }

extern bool MAP_MODE_DEBUG;

/**
 *
 */
class MapMode : public rpg_mode_manager::GameMode
{
  friend class rpg_script::ScriptEngine;
public:
  /**
   * Default constructor
   */
  MapMode(const std::string& _lua_filepath);

  /**
   * Destructor
   */
  ~MapMode()
  {}

  /**
   * Update the MapMode object.
   */
  void Update();

  /**
   * Draw whatever is visible in the map.
   */
  void Draw();

  /**
   * Reset the current game state.
   */
  void Reset();

private:
  /**
   * Load the map from the specified Lua file.
   * @return True if successful, false otherwise
   */
  bool LoadMap();

  /**
   * Delete the copy functions
   */
  MapMode(const MapMode& _copy) = delete;
  MapMode& operator=(MapMode& _copy) = delete;

  // File path to the Lua file with the map data
  std::string lua_filepath;

  // Vector containing the map layers for the mode
  // std::vector<private_map_mode::MapLayer> layers;

  //
  private_map_mode::TileSupervisor* tile_supervisor;

  //
  private_map_mode::ObjectSupervisor* object_supervisor;

  //
  private_map_mode::MapFrame map_frame;
};

}

#endif //MAP_MODE_HPP
