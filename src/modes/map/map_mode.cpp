#include "../../utils/globals.hpp"
#include "map_mode.hpp"

#include "../../core/script.hpp"
#include "../../core/video_manager.hpp"
#include "../../core/input.hpp"
#include "../../core/global/global.hpp"
#include "../../core/global/global_actors.hpp"
#include "map_tiles.hpp"
#include "map_objects.hpp"
#include "map_utils.hpp"
  using rpg_map_mode::private_map_mode::MapRectangle;

//#include "../../utils/selene.hpp"
//using namespace sel;

namespace rpg_map_mode
{

bool MAP_MODE_DEBUG = true;

MapMode::MapMode(const string& _lua_filepath)
  : GameMode()
  , lua_filepath(_lua_filepath)
  , tile_supervisor(nullptr)
{
  IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "MapMode constructor called" << endl;

  tile_supervisor = new private_map_mode::TileSupervisor();

  if (!LoadMap())
    PRINT_ERROR << "Failed to load Lua tilemap: " << lua_filepath << endl;
}

void MapMode::Update()
{
  rpg_global::GlobalManager->Update();

  rpg_global::GlobalCharacter* hero = rpg_global::GlobalManager->GetHero();
  if (!hero)
    return;

  sf::Vector2f char_pos = hero->GetPosition();
  sf::Vector2i curr_tile(char_pos.x / 32, char_pos.y / 32);
  MapRectangle char_rect(char_pos.x, char_pos.y,
                         char_pos.x + 32, char_pos.y + 32);

  if (rpg_input::InputManager->IsUpKeyPressed())
  {
    sf::Vector2i check_tile(curr_tile.x, curr_tile.y - 1);
    MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
                           check_tile.x * 32 + 32, check_tile.y * 32 + 32);
    if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
        !MapRectangle::CheckIntersection(char_rect, tile_rect))
      hero->Move(0, -2);
  }
  if (rpg_input::InputManager->IsDownKeyPressed())
  {
    sf::Vector2i check_tile(curr_tile.x, curr_tile.y + 1);
    MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
                           check_tile.x * 32 + 32, check_tile.y * 32 + 32);
    if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
        !MapRectangle::CheckIntersection(char_rect, tile_rect))
      hero->Move(0, 2);
  }
  if (rpg_input::InputManager->IsLeftKeyPressed())
  {
    sf::Vector2i check_tile(curr_tile.x - 1, curr_tile.y);
    MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
                           check_tile.x * 32 + 32, check_tile.y * 32 + 32);
    if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
        !MapRectangle::CheckIntersection(char_rect, tile_rect))
      hero->Move(-2, 0);
  }
  if (rpg_input::InputManager->IsRightKeyPressed())
  {
    sf::Vector2i check_tile(curr_tile.x + 1, curr_tile.y);
    MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
                           check_tile.x * 32 + 32, check_tile.y * 32 + 32);
    if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
        !MapRectangle::CheckIntersection(char_rect, tile_rect))
      hero->Move(2, 0);
  }
}

void MapMode::Draw()
{
  tile_supervisor->DrawLayers();
}

void MapMode::Reset()
{

}

bool MapMode::LoadMap()
{
  IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "Loading tilemap: " << lua_filepath << endl;

  rpg_script::ScriptManager->LoadTilemap(lua_filepath, this);

  // State state(true);
  // if (!state.Load(lua_filepath))
  // {
  //   PRINT_ERROR << "Failed to open load Lua file: " << lua_filepath << endl;
  //   return false;
  // }
  //
  // auto map_data = state["map_data"];
  //
  //
  // if (!tile_supervisor->Load(map_data))
  // {
  //   PRINT_ERROR << "Failed to load tiles in TileSupervisor" << endl;
  //   return false;
  // }

  return true;
}

}
