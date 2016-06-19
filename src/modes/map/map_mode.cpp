#include "../../utils/globals.hpp"
#include "map_mode.hpp"

#include "../../core/script_read.hpp"
#include "../../core/video_manager.hpp"
#include "../../core/input.hpp"
#include "../../core/global/global.hpp"
#include "../../core/global/global_actors.hpp"
#include "map_tiles.hpp"
#include "map_objects.hpp"
#include "map_sprites.hpp"
#include "map_events.hpp"
#include "map_utils.hpp"
  using namespace rpg_input;
  using rpg_map_mode::private_map_mode::MapRectangle;
  using rpg_map_mode::private_map_mode::MapLayerType;

namespace rpg_map_mode
{

MapMode *MapMode::current_instance = nullptr;
bool MAP_MODE_DEBUG = true;

MapMode::MapMode(const string& _map_name)
  : GameMode()
  , map_name(_map_name)
  , camera(nullptr)
  , delta_x(0)
  , delta_y(0)
  , tile_supervisor(nullptr)
  , temp(nullptr)
  , enemy(nullptr)
  , read_script(nullptr)
{
  IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "MapMode constructor called" << endl;

  current_instance = this;

  tile_supervisor = new private_map_mode::TileSupervisor();
  object_supervisor = new private_map_mode::ObjectSupervisor();
  event_supervisor = new private_map_mode::EventSupervisor();

  // temp = new private_map_mode::MapSprite(MapObjectDrawLayer::GROUND_OBJECT);
  // if (!temp->LoadAnimations("data/entities/actor0-walking.lua"))
  // {
  //   PRINT_ERROR << "Failed to load animations for character" << endl;
  //   delete temp;
  //   temp = nullptr;
  // }
  // temp->SetCurrentAnimation("idle-south");
  // temp->SetDirection(DIRECTION_SOUTH);
  // temp->SetCenterPosition(128, 128);
  // temp->SetDimensions(32, 32);
  // // object_supervisor->RegisterObject(temp);
  // camera = temp;

  // enemy = private_map_mode::EnemySprite::Create();
  // if (!enemy->LoadAnimations("data/entities/enemy0-walking.lua"))
  // {
  //   PRINT_ERROR << "Failed to load animations for character" << endl;
  //   delete enemy;
  //   enemy = nullptr;
  // }
  // enemy->SetCurrentAnimation("idle-south");
  // enemy->SetDirection(DIRECTION_SOUTH);
  // enemy->SetCenterPosition(320, 320);
  // enemy->SetDimensions(32, 32);
  // object_supervisor->RegisterObject(enemy);

  //camera_timer.InitTimer(0, 1);

  //camera = new private_map_mode::Camera(sf::Vector2i(32, 32));

  //camera->SetCenterPosition(128, 128);

  if (!LoadMap())
    PRINT_ERROR << "Failed to load Lua tilemap: " << map_name << endl;
}

MapMode::~MapMode()
{
  delete temp; temp = nullptr;
  //delete camera; camera = nullptr;
  delete tile_supervisor; tile_supervisor = nullptr;
  delete read_script; read_script = nullptr;
}

void MapMode::SetCamera(private_map_mode::VirtualSprite* _sprite, const float _duration)
{
  // if (camera == _sprite)
  //   return;
  //
  // if (_duration > 0)
  // {
  //   delta_x = camera->GetPosition().x - _sprite->GetPosition().x;
  //   delta_y = camera->GetPosition().y - _sprite->GetPosition().y;
  //   camera_timer.Reset();
  //   camera_timer.SetDuration(_duration);
  //   camera_timer.Run();
  // }
  camera = _sprite;
}

void MapMode::Update()
{
  if (!camera)
    return;

  UpdateCameraFrame();

  tile_supervisor->Update();
  object_supervisor->Update();
  //camera_timer.Update();

  read_script->CallFunction("Update");
  if (read_script->HasError())
    read_script->PrintErrors();

  if (!InputManager->IsUpKeyPressed() && !InputManager->IsDownKeyPressed() &&
      !InputManager->IsLeftKeyPressed() && !InputManager->IsRightKeyPressed())
    camera->SetMoving(false);
  else
    camera->SetMoving(true);

  if (camera->IsMoving())
  {
    if (InputManager->IsUpKeyPressed())
    {
      if (InputManager->IsLeftKeyPressed())
        camera->SetDirection(DIRECTION_NORTHWEST);
      else if (InputManager->IsRightKeyPressed())
        camera->SetDirection(DIRECTION_NORTHEAST);
      else
        camera->SetDirection(DIRECTION_NORTH);
    }
    else if (InputManager->IsDownKeyPressed())
    {
      if (InputManager->IsLeftKeyPressed())
        camera->SetDirection(DIRECTION_SOUTHWEST);
      else if (InputManager->IsRightKeyPressed())
        camera->SetDirection(DIRECTION_SOUTHEAST);
      else
        camera->SetDirection(DIRECTION_SOUTH);
    }
    else if (InputManager->IsLeftKeyPressed())
      camera->SetDirection(DIRECTION_WEST);
    else if (InputManager->IsRightKeyPressed())
      camera->SetDirection(DIRECTION_EAST);
  }

  event_supervisor->Update();

  GameMode::Update();

  // < TEMPORARY STUFF BEGIN
  // rpg_global::GlobalCharacter* hero = rpg_global::GlobalManager->GetHero();
  // if (!hero)
  //   return;
  //
  // sf::Vector2i char_pos = camera->GetPosition() - sf::Vector2i(16, 16);//camera->GetCenterPosition() - sf::Vector2f(16, 16);
  // sf::Vector2i position_delta = sf::Vector2i(0, 0);
  //
  // temp->Update();
  //
  // sf::Vector2i curr_tile(char_pos.x / 32, char_pos.y / 32);
  // MapRectangle char_rect(char_pos.x, char_pos.y,
  //                        char_pos.x + 32, char_pos.y + 32);
  //
  // // Set Idle Animations
  // if (rpg_input::InputManager->NoMovementKeysPressed())
  // {
  //   string direction = temp->GetDirection();
  //   temp->SetCurrentAnimation("idle-" + direction);
  // }
  //
  // if (rpg_input::InputManager->IsUpKeyPressed())
  // {
  //   sf::Vector2i check_tile(curr_tile.x, curr_tile.y - 1);
  //   MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
  //                          check_tile.x * 32 + 32, check_tile.y * 32 + 32);
  //   if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
  //       !MapRectangle::CheckIntersection(char_rect, tile_rect))
  //   {
  //     temp->SetCurrentAnimation("walk-north");
  //     temp->SetDirection("north");
  //     temp->SetMoving(true);
  //     //position_delta.y -= 5;
  //   }
  // }
  // if (rpg_input::InputManager->IsDownKeyPressed())
  // {
  //   sf::Vector2i check_tile(curr_tile.x, curr_tile.y + 1);
  //   MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
  //                          check_tile.x * 32 + 32, check_tile.y * 32 + 32);
  //   if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
  //       !MapRectangle::CheckIntersection(char_rect, tile_rect))
  //   {
  //     temp->SetCurrentAnimation("walk-south");
  //     temp->SetDirection("south");
  //     temp->SetMoving(true);
  //     //position_delta.y += 5;
  //   }
  // }
  // if (rpg_input::InputManager->IsLeftKeyPressed())
  // {
  //   sf::Vector2i check_tile(curr_tile.x - 1, curr_tile.y);
  //   MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
  //                          check_tile.x * 32 + 32, check_tile.y * 32 + 32);
  //   if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
  //       !MapRectangle::CheckIntersection(char_rect, tile_rect))
  //   {
  //     temp->SetCurrentAnimation("walk-west");
  //     temp->SetDirection("west");
  //     temp->SetMoving(true);
  //     //position_delta.x -= 5;
  //   }
  // }
  // if (rpg_input::InputManager->IsRightKeyPressed())
  // {
  //   sf::Vector2i check_tile(curr_tile.x + 1, curr_tile.y);
  //   MapRectangle tile_rect(check_tile.x * 32, check_tile.y * 32,
  //                          check_tile.x * 32 + 32, check_tile.y * 32 + 32);
  //
  //   if (!object_supervisor->IsMapCollision(check_tile.x, check_tile.y) ||
  //       !MapRectangle::CheckIntersection(char_rect, tile_rect))
  //   {
  //     temp->SetCurrentAnimation("walk-east");
  //     temp->SetDirection("east");
  //     temp->SetMoving(true);
  //     //position_delta.x += 5;
  //   }
  // }
  //
  // //camera->SetCenterPosition(char_pos.x + position_delta.x + 16, char_pos.y + position_delta.y + 16);
  //temp->Update();
}

float MapMode::GetScreenXCoordinate(float _tile_position_x) const
{
    return _tile_position_x - frame.screen_edges.left;
}

float MapMode::GetScreenYCoordinate(float _tile_position_y) const
{
    return _tile_position_y - frame.screen_edges.top;
}

void MapMode::Draw()
{
  tile_supervisor->DrawLayers(frame, MapLayerType::GROUND);

  if (rpg_video::VideoManager->IsDebug())
    object_supervisor->DrawMapZones();

  object_supervisor->DrawObjects();

  tile_supervisor->DrawLayers(frame, MapLayerType::WALL);
  tile_supervisor->DrawLayers(frame, MapLayerType::SKY);

}

void MapMode::DrawGrid()
{
  rpg_video::VideoManager->DrawGrid(32, 32,
                                    1024, 768,
                                    32, 32, 1, sf::Color::Red);
}

void MapMode::Reset()
{

}

bool MapMode::LoadMap()
{
  std::string script_name = "data/maps/" + map_name + "-map.lua";

  if (!read_script)
    read_script = new rpg_script::ReadScript();

  if (read_script->IsOpen())
    read_script->CloseFile();

  if (!read_script->OpenFile(script_name))
  {
    PRINT_ERROR << "Failed to open tilemap script: " << script_name << endl;
    return false;
  }

  // cout << "---------- Tilemap Information ----------" << endl;
  // cout << "| File: " << _lua_filepath << endl;

  read_script->OpenTable("map_data");

  string script_path = read_script->ReadData<string>("script_path", "");

  int map_width = read_script->ReadData<int>("num_cols", -1);
  int map_height = read_script->ReadData<int>("num_rows", -1);
  if (map_width < 0 || map_height < 0)
  {
    PRINT_ERROR << "Invalid map dimensions in map script." << endl;
    return false;
  }

  // string name = read_script->ReadData<string>("name", "");
  // cout << "| Name: " << name << endl;
  // cout << "|" << endl;
  //
  // cout << "| Dimensions: " << map_width << " x " << map_height << endl;
  // cout << "|\n---------- Tileset Information ----------" << endl;

  if (!tile_supervisor)
    tile_supervisor = new private_map_mode::TileSupervisor();

  read_script->OpenTable("tilesets");
  int num_tilesets = read_script->ReadData<int>("count", -1);

  // cout << "| Tileset Count: " << num_tilesets << endl;

  if (num_tilesets < 0)
  {
    PRINT_ERROR << "No tilesets found in map script." << endl;
    return false;
  }

  for (int i = 0; i < num_tilesets; ++i)
  {
    string path = read_script->ReadData<string>(i, "");
    // cout << "| " << i << ". " << path << endl;
    if (path.empty() || !LoadTileset(path))
    {
      PRINT_ERROR << "Failed to load tileset" << endl;
      return false;
    }
  }
  read_script->CloseTable();

  // cout << "|\n----------- Layer Information -----------" << endl;

  // Load in the layer data
  read_script->OpenTable("layers");
  int num_layers = read_script->ReadData<int>("num_layers", -1);

  // cout << "| Layer Count: " << num_layers << endl;
  if (num_layers < 0)
  {
    PRINT_ERROR << "No layers found in map script." << endl;
    return false;
  }

  if (read_script->HasError())
  {
    read_script->PrintErrors();
    return false;
  }

  for (int l = 0; l < num_layers; ++l)
  {
    read_script->OpenTableIntegers(l);

    MapLayerType type;
    string str_type = read_script->ReadData<string>("type", "");

    if (str_type == "ground")
      type = MapLayerType::GROUND;
    else if (str_type == "wall")
      type = MapLayerType::WALL;
    else if (str_type == "sky")
      type = MapLayerType::SKY;
    else
    {
      PRINT_WARNING << "Unknown type for layer" << endl;
      continue;
    }

    // cout << "| " << l << ". " << str_type << endl;

    private_map_mode::MapLayer layer(type);
    for (int row = 0; row < map_height; ++row)
    {
      read_script->OpenTableIntegers(row + 1);
      // cout << "|\t";
      vector<int> temp;
      for (int col = 0; col < map_width; ++col)
      {
        int id = read_script->ReadData<int>(col + 1, -1);
        // cout << setw(3) << id << " ";
        temp.push_back(id);
      }
      // cout << endl;
      layer.tiles.push_back(temp);
      read_script->CloseTable();
    }

    tile_supervisor->layers.push_back(layer);

    read_script->CloseTable();
  }
  read_script->CloseTable();

  // cout << "|\n--------- Collision Information ---------" << endl;
  // Load object data
  if (!object_supervisor)
    object_supervisor = new private_map_mode::ObjectSupervisor();

  // cout << "| " << "Static Collision Matrix" << endl;

  read_script->OpenTable("collision");

  if (read_script->HasError())
  {
    read_script->PrintErrors();
    return false;
  }

  for (int i = 0; i < map_height; ++i)
  {
    read_script->OpenTableIntegers(i + 1);
    // cout << "|\t";
    vector<int> temp;
    for (int j = 0; j < map_width; ++j)
    {
      int value = read_script->ReadData<int>(j + 1, -1);
      // cout << setw(2) << value << " ";
      temp.push_back(value);
    }
    object_supervisor->collision_grid.push_back(temp);
    // cout << endl;
    read_script->CloseTable();
  }

  if (read_script->HasError())
  {
      read_script->PrintErrors();
      return false;
  }

  read_script->CloseTable();
  read_script->CloseFile();

  // cout << "-----------------------------------------" << endl;

  if (script_path.empty())
    return false;

  if (!read_script->OpenFile(script_path))
  {
    PRINT_ERROR << "Failed to open tilemap script: " << script_path << endl;
    return false;
  }

  IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "Loading tilemap functionality script" << endl;

  read_script->CallFunction("Load");
  if (read_script->HasError())
  {
      read_script->PrintErrors();
      return false;
  }

  // read_script->CloseFile();

  return true;
}

bool MapMode::LoadTileset(const string& _lua_filepath)
{
  rpg_script::ReadScript tileset_script;
  if (!tileset_script.OpenFile(_lua_filepath))
  {
    PRINT_ERROR << "Failed to open tileset script: " << _lua_filepath << endl;
    return false;
  }

  tileset_script.OpenTable("tileset_data");

  string image_name = tileset_script.ReadData<string>("name", "");
  string image_path = tileset_script.ReadData<string>("image_path", "");

  // cout << "|\tImage Name: " << image_name << endl;
  // cout << "|\tImage Path: " << image_path << endl;

  int margin = tileset_script.ReadData<int>("margin", -1);
  int spacing = tileset_script.ReadData<int>("spacing", -1);
  int num_cols = tileset_script.ReadData<int>("num_cols", -1);
  int num_rows = tileset_script.ReadData<int>("num_rows", -1);

  int tile_width = tileset_script.ReadData<int>("tile_width", -1);
  int tile_height = tileset_script.ReadData<int>("tile_height", -1);

  // cout << "|\tMargin / Spacing: " << margin << " / " << spacing << endl;
  // cout << "|\tSize: " << num_cols << " x " << num_rows << endl;
  // cout << "|\tTile Size: " << tile_width << " x " << tile_height << endl;

  if (tileset_script.HasError())
  {
    tileset_script.PrintErrors();
    return false;
  }

  for (int row = 0; row < num_rows; ++row)
  {
    for (int col = 0; col < num_cols; ++col)
    {
      sf::IntRect tile_rect(margin + spacing * col + col * tile_width,
                            margin + spacing * row + row * tile_height,
                            32, 32);

      int loaded_id = rpg_resource::ResourceManager->LoadImageRect(image_path, tile_rect);
      if (loaded_id >= 0)
        tile_supervisor->tile_ids.push_back(loaded_id);
      else
      {
        PRINT_ERROR << "Failed to load tile from image" << endl;
        return false;
      }
    }
  }
  return true;
}

void MapMode::UpdateCameraFrame()
{
  sf::Vector2i position = camera->GetCenterPosition();

  frame.tile_x_offset = static_cast<int>(position.x) % 32;
  frame.tile_y_offset = static_cast<int>(position.y) % 32;

  frame.tile_x_start = static_cast<signed int>(position.x / 32 - rpg_video::VideoManager->GetScreenWidth() / 64);
  frame.tile_y_start = static_cast<signed int>(position.y / 32 - rpg_video::VideoManager->GetScreenHeight() / 64);

  frame.screen_edges.left = (position.x) - rpg_video::VideoManager->GetScreenWidth() / 2;
  frame.screen_edges.right = (position.x) + rpg_video::VideoManager->GetScreenWidth() / 2;
  frame.screen_edges.top = (position.y) - rpg_video::VideoManager->GetScreenHeight() / 2;
  frame.screen_edges.bottom = (position.y) + rpg_video::VideoManager->GetScreenHeight() / 2;

  frame.num_draw_x_axis = static_cast<int>(rpg_video::VideoManager->GetScreenWidth() / 32) + 3;
  frame.num_draw_y_axis = static_cast<int>(rpg_video::VideoManager->GetScreenHeight() / 32) + 3;
}

}
