#include "../../utils/globals.hpp"
#include "map_mode.hpp"

#include "../../core/script.hpp"
#include "../../core/video_manager.hpp"
#include "../../core/input.hpp"
#include "../../core/global/global.hpp"
#include "../../core/global/global_actors.hpp"
#include "map_tiles.hpp"
#include "map_objects.hpp"
#include "map_sprites.hpp"
#include "map_camera.hpp"
#include "map_utils.hpp"
  using rpg_map_mode::private_map_mode::MapRectangle;
  using rpg_map_mode::private_map_mode::MapLayerType;

namespace rpg_map_mode
{

bool MAP_MODE_DEBUG = true;

MapMode::MapMode(const string& _lua_filepath)
  : GameMode()
  , lua_filepath(_lua_filepath)
  , camera(nullptr)
  , tile_supervisor(nullptr)
  , temp_sprite(nullptr)
{
  IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "MapMode constructor called" << endl;

  tile_supervisor = new private_map_mode::TileSupervisor();
  camera = new private_map_mode::Camera(sf::Vector2i(32, 32));

  camera->SetCenterPosition(128, 128);

  if (!LoadMap())
    PRINT_ERROR << "Failed to load Lua tilemap: " << lua_filepath << endl;
}

MapMode::~MapMode()
{
  delete camera; camera = nullptr;
  delete tile_supervisor; tile_supervisor = nullptr;
}

void MapMode::Update()
{
  rpg_global::GlobalCharacter* hero = rpg_global::GlobalManager->GetHero();
  if (!hero)
    return;

  sf::Vector2f char_pos = camera->GetCenterPosition() - sf::Vector2f(16, 16);
  sf::Vector2i position_delta = sf::Vector2i(0, 0);

  rpg_global::GlobalManager->Update();

  sf::Vector2i curr_tile(char_pos.x / 32, char_pos.y / 32);
  MapRectangle char_rect(char_pos.x, char_pos.y,
                         char_pos.x + 32, char_pos.y + 32);

  // Set Idle Animations
  if (rpg_input::InputManager->NoMovementKeysPressed())
  {
    string direction = hero->GetDirection();
    hero->SetCurrentAnimation("idle-" + direction);
  }

  if (rpg_input::InputManager->IsUpKeyPressed())
  {
    sf::Vector2i check_tile(curr_tile.x, curr_tile.y - 1);
    MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
                           check_tile.x * 32 + 32, check_tile.y * 32 + 32);
    if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
        !MapRectangle::CheckIntersection(char_rect, tile_rect))
    {
      hero->SetCurrentAnimation("walk-north");
      hero->SetDirection("north");
      position_delta.y -= 5;
    }
  }
  if (rpg_input::InputManager->IsDownKeyPressed())
  {
    sf::Vector2i check_tile(curr_tile.x, curr_tile.y + 1);
    MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
                           check_tile.x * 32 + 32, check_tile.y * 32 + 32);
    if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
        !MapRectangle::CheckIntersection(char_rect, tile_rect))
    {
      hero->SetCurrentAnimation("walk-south");
      hero->SetDirection("south");
      position_delta.y += 5;
    }
  }
  if (rpg_input::InputManager->IsLeftKeyPressed())
  {
    sf::Vector2i check_tile(curr_tile.x - 1, curr_tile.y);
    MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
                           check_tile.x * 32 + 32, check_tile.y * 32 + 32);
    if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
        !MapRectangle::CheckIntersection(char_rect, tile_rect))
    {
      hero->SetCurrentAnimation("walk-west");
      hero->SetDirection("west");
      position_delta.x -= 5;
    }
  }
  if (rpg_input::InputManager->IsRightKeyPressed())
  {
    sf::Vector2i check_tile(curr_tile.x + 1, curr_tile.y);
    MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
                           check_tile.x * 32 + 32, check_tile.y * 32 + 32);

    if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
        !MapRectangle::CheckIntersection(char_rect, tile_rect))
    {
      hero->SetCurrentAnimation("walk-east");
      hero->SetDirection("east");
      position_delta.x += 5;
    }
  }

  camera->SetCenterPosition(char_pos.x + position_delta.x + 16, char_pos.y + position_delta.y + 16);
  camera->Update();
}

void MapMode::Draw()
{
  tile_supervisor->DrawLayers(camera->GetCameraBounds(), camera->GetTileOffset(), MapLayerType::GROUND);

  rpg_global::GlobalManager->DrawGlobalCharacters();

  tile_supervisor->DrawLayers(camera->GetCameraBounds(), camera->GetTileOffset(), MapLayerType::WALL);
  tile_supervisor->DrawLayers(camera->GetCameraBounds(), camera->GetTileOffset(), MapLayerType::SKY);
}

void MapMode::Reset()
{

}

bool MapMode::LoadMap()
{
  IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "Loading tilemap: " << lua_filepath << endl;

  rpg_script::ScriptManager->LoadTilemap(lua_filepath, this);

  // TODO: Rewrite tilemap loading code in here. Abstract it away from the ScriptManager.
  // It doesn't make sense in it's current place.

  return true;
}

}
