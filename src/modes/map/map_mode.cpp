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
#include "map_combat.hpp"
#include "map_utils.hpp"
  using namespace rpg_input;
  using namespace rpg_script;
  using namespace rpg_map_mode::private_map_mode;
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
  , event_supervisor(nullptr)
  , read_script(nullptr)
  , active(true)
{
  IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "MapMode constructor called" << endl;

  current_instance = this;

  tile_supervisor = new TileSupervisor();
  object_supervisor = new ObjectSupervisor();
  event_supervisor = new EventSupervisor();

  read_script = new ReadScript();

  if (!LoadMap())
    PRINT_ERROR << "Failed to load Lua tilemap: " << map_name << endl;
}

MapMode::~MapMode()
{
  delete tile_supervisor; tile_supervisor = nullptr;
  delete object_supervisor; object_supervisor = nullptr;
  delete event_supervisor; event_supervisor = nullptr;
  delete read_script; read_script = nullptr;
}

void MapMode::SetCamera(private_map_mode::VirtualSprite* _sprite, const float _duration)
{
  camera = _sprite;
}

void MapMode::Update()
{
  if (!camera)
    return;

  UpdateCameraFrame();

  tile_supervisor->Update();
  object_supervisor->Update();

  // Call the Lua Update() function
  read_script->CallFunction("Update");
  if (read_script->HasError())
    read_script->PrintErrors();

  if (InputManager->IsAttackKeyPressed())
    HandleAttacking();

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
}

void MapMode::HandleAttacking()
{
  if (!camera->IsFinishedAttacking())
    return;

  camera->SetAttacking(true);
  MapObject* closest_obj = object_supervisor->FindClosestObject(camera, ObjectType::ENEMY_TYPE, 3);
  if (!closest_obj)
    return;

  EnemySprite* closest_enemy = reinterpret_cast<EnemySprite*>(closest_obj);

  cout << "Attacking " << closest_enemy->GetObjectId() << endl;
  closest_enemy->TakeDamage(10);
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
    read_script = new ReadScript();

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

void MapMode::Deactivate()
{
  if (!active)
    return;

  active = false;
}

}
