#ifndef MAP_MODE_HPP
#define MAP_MODE_HPP

#include "../../core/mode_manager.hpp"
#include "../../core/system.hpp"
#include "map_utils.hpp"

namespace rpg_script
{
class ScriptEngine;
class ReadScript;
}

namespace rpg_map_mode
{

namespace private_map_mode
{
class VirtualSprite;
class MapSprite;
class EnemySprite;
class TileSupervisor;
class ObjectSupervisor;
class EventSupervisor;
class MapFrame;
class Camera;
}

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
  ~MapMode();

  static MapMode* CurrentInstance()
  {
    return current_instance;
  }

  private_map_mode::TileSupervisor* GetTileSupervisor() const
  {
    return tile_supervisor;
  }

  private_map_mode::ObjectSupervisor* GetObjectSupervisor() const
  {
    return object_supervisor;
  }

  private_map_mode::EventSupervisor* GetEventSupervisor() const
  {
    return event_supervisor;
  }

  void SetCamera(private_map_mode::VirtualSprite* _sprite, const float _duration);

  private_map_mode::VirtualSprite* GetCamera() const
  {
    return camera;
  }

  private_map_mode::MapFrame GetWindowFrame() const
  {
    return frame;
  }

  // void SetCamera(private_map_mode::VirtualSprite* _sprite)
  // {
  //   camera = _sprite;
  // }

  /**
   * Update the MapMode object.
   */
  void Update();

  void HandleAttacking();

  float GetScreenXCoordinate(float tile_position_x) const;
  float GetScreenYCoordinate(float tile_position_y) const;

  /**
   * Draw whatever is visible in the map.
   */
  void Draw();

  void DrawGrid();

  /**
   * Reset the current game state.
   */
  void Reset();

  void Deactivate();

private:
  /**
   * Load the map from the specified Lua file.
   * @return True if successful, false otherwise
   */
  bool LoadMap();

  bool LoadTileset(const std::string& _lua_filepath);

  void UpdateCameraFrame();
  // sf::IntRect GetCameraBounds();
  //
  // sf::Vector2i GetCameraOffset();

  /**
   * Delete the copy functions
   */
  MapMode(const MapMode& _copy) = delete;
  MapMode& operator=(MapMode& _copy) = delete;

  static MapMode* current_instance;

  // File path to the Lua file with the map data
  std::string map_name;

  private_map_mode::VirtualSprite* camera;

  rpg_system::SystemTimer camera_timer;

  float delta_x;

  float delta_y;

  //
  private_map_mode::TileSupervisor* tile_supervisor;

  //
  private_map_mode::ObjectSupervisor* object_supervisor;

  private_map_mode::EventSupervisor* event_supervisor;

  //
  private_map_mode::MapFrame frame;

  rpg_script::ReadScript* read_script;

  bool active;
};

}

#endif //MAP_MODE_HPP
