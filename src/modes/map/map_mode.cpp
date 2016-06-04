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
#include "map_utils.hpp"
  using namespace rpg_input;
  using rpg_map_mode::private_map_mode::MapRectangle;
  using rpg_map_mode::private_map_mode::MapLayerType;

namespace rpg_map_mode
{

MapMode *MapMode::current_instance = nullptr;
bool MAP_MODE_DEBUG = true;

MapMode::MapMode(const string& _lua_filepath)
  : GameMode()
  , lua_filepath(_lua_filepath)
  , camera(nullptr)
  , delta_x(0)
  , delta_y(0)
  , tile_supervisor(nullptr)
  , temp(nullptr)
{
  IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "MapMode constructor called" << endl;

  current_instance = this;

  tile_supervisor = new private_map_mode::TileSupervisor();
  object_supervisor = new private_map_mode::ObjectSupervisor();
  temp = new private_map_mode::MapSprite(MapObjectDrawLayer::GROUND_OBJECT);
  if (!temp->LoadAnimations("data/gchar/actor0-walking.lua"))
  {
    PRINT_ERROR << "Failed to load animations for character" << endl;
    delete temp;
    temp = nullptr;
  }
  temp->SetCurrentAnimation("idle-south");
  temp->SetDirection(DIRECTION_SOUTH);
  temp->SetPosition(64, 64);
  object_supervisor->RegisterObject(temp);
  camera = temp;

  //camera_timer.InitTimer(0, 1);

  //camera = new private_map_mode::Camera(sf::Vector2i(32, 32));

  //camera->SetCenterPosition(128, 128);

  if (!LoadMap(_lua_filepath))
    PRINT_ERROR << "Failed to load Lua tilemap: " << lua_filepath << endl;
}

MapMode::~MapMode()
{
  delete temp; temp = nullptr;
  //delete camera; camera = nullptr;
  delete tile_supervisor; tile_supervisor = nullptr;
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
  UpdateCameraFrame();

  tile_supervisor->Update();
  object_supervisor->Update();

  //camera_timer.Update();

  if (!camera)
    return;
  camera->Update();

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
    _tile_position_x = _tile_position_x - frame.screen_edges.left;
        //* 1024 / private_map_mode::SCREEN_GRID_Y_LENGTH;
    //_tile_position_x = _tile_position_x + rpg_video::VideoManager->GetScreenHeight() / 2;
    //tile_position_x = FloorToFloatMultiple(tile_position_x, GetMapPixelXLength());
    return _tile_position_x;
}

float MapMode::GetScreenYCoordinate(float _tile_position_y) const
{
    _tile_position_y = _tile_position_y - frame.screen_edges.top;
      //* 768 / private_map_mode::SCREEN_GRID_Y_LENGTH;
    //tile_position_y = FloorToFloatMultiple(tile_position_y, GetMapPixelYLength());
    //_tile_position_y = rpg_video::VideoManager->GetScreenHeight() / 2 - _tile_position_y;
    return _tile_position_y;
}

void MapMode::Draw()
{

  tile_supervisor->DrawLayers(frame, MapLayerType::GROUND);

  object_supervisor->DrawObjects();

  tile_supervisor->DrawLayers(frame, MapLayerType::WALL);
  tile_supervisor->DrawLayers(frame, MapLayerType::SKY);

  //DrawGrid();
}

void MapMode::DrawGrid()
{
  rpg_video::VideoManager->DrawGrid(32, 32,
                         1024, 768,
                         32, 32, 1, sf::Color::Red);


  // float x = _map_frame.tile_x_offset * GRID_LENGTH;
  // float y = _map_frame.tile_y_offset * GRID_LENGTH;
  // VideoManager->Move(x, y);
  //
  // // Calculate the dimensions of the grid.
  // float left = VideoManager->GetCoordSys().GetLeft();
  // float right = VideoManager->GetCoordSys().GetRight();
  // float top = VideoManager->GetCoordSys().GetTop();
  // float bottom = VideoManager->GetCoordSys().GetBottom();
  //
  // // Calculate the dimensions of the grid's cells.
  // float width_cell_horizontal = (right - left) / SCREEN_GRID_X_LENGTH;
  // float width_cell_vertical = (bottom - top) / SCREEN_GRID_Y_LENGTH;
  //
  // // Increase the dimensions of the grid to prevent clipping around its edges.
  // left -= (width_cell_horizontal * 2.0f);
  // right += (width_cell_horizontal * 2.0f);
  // top -= (width_cell_vertical * 2.0f);
  // bottom += (width_cell_vertical * 2.0f);
  //
  // // Draw the collision grid.
  // Color color = Color(0.0f, 0.0f, 0.5f, 0.2f);
  // VideoManager->DrawGrid(left, top, right, bottom, width_cell_horizontal, width_cell_vertical, 2, color);
  //
  // // Draw the tile grid.
  // color = Color(0.5f, 0.0f, 0.0f, 0.3f);
  // VideoManager->DrawGrid(left, top, right, bottom, width_cell_horizontal * 2.0f, width_cell_vertical * 2.0f, 2, color);
  //
  // VideoManager->PopMatrix();
}

void MapMode::Reset()
{

}

bool MapMode::LoadMap(const std::string& _lua_filepath)
{
  rpg_script::ReadScript map_script;
  if (!map_script.OpenFile(_lua_filepath))
  {
    PRINT_ERROR << "Failed to open tilemap script: " << _lua_filepath << endl;
    return false;
  }

  cout << "---------- Tilemap Information ----------" << endl;
  cout << "| File: " << _lua_filepath << endl;

  map_script.OpenTable("map_data");

  int map_width = map_script.ReadData<int>("num_cols", -1);
  int map_height = map_script.ReadData<int>("num_rows", -1);
  if (map_width < 0 || map_height < 0)
  {
    PRINT_ERROR << "Invalid map dimensions in map script." << endl;
    return false;
  }

  string name = map_script.ReadData<string>("name", "");
  cout << "| Name: " << name << endl;
  cout << "|" << endl;

  cout << "| Dimensions: " << map_width << " x " << map_height << endl;
  cout << "|\n---------- Tileset Information ----------" << endl;

  if (!tile_supervisor)
    tile_supervisor = new private_map_mode::TileSupervisor();

  map_script.OpenTable("tilesets");
  int num_tilesets = map_script.ReadData<int>("count", -1);

  cout << "| Tileset Count: " << num_tilesets << endl;

  if (num_tilesets < 0)
  {
    PRINT_ERROR << "No tilesets found in map script." << endl;
    return false;
  }

  for (int i = 0; i < num_tilesets; ++i)
  {
    string path = map_script.ReadData<string>(i, "");
    cout << "| " << i << ". " << path << endl;
    if (path.empty() || !LoadTileset(path))
    {
      PRINT_ERROR << "Failed to load tileset" << endl;
      return false;
    }
  }
  map_script.CloseTable();

  cout << "|\n----------- Layer Information -----------" << endl;

  // Load in the layer data
  map_script.OpenTable("layers");
  int num_layers = map_script.ReadData<int>("num_layers", -1);

  cout << "| Layer Count: " << num_layers << endl;
  if (num_layers < 0)
  {
    PRINT_ERROR << "No layers found in map script." << endl;
    return false;
  }

  if (map_script.HasError())
  {
    map_script.PrintErrors();
    return false;
  }

  for (int l = 0; l < num_layers; ++l)
  {
    map_script.OpenTableIntegers(l);

    MapLayerType type;
    string str_type = map_script.ReadData<string>("type", "");

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

    cout << "| " << l << ". " << str_type << endl;

    private_map_mode::MapLayer layer(type);
    for (int row = 0; row < map_height; ++row)
    {
      map_script.OpenTableIntegers(row + 1);
      cout << "|\t";
      vector<int> temp;
      for (int col = 0; col < map_width; ++col)
      {
        int id = map_script.ReadData<int>(col + 1, -1);
        cout << setw(3) << id << " ";
        temp.push_back(id);
      }
      cout << endl;
      layer.tiles.push_back(temp);
      map_script.CloseTable();
    }

    tile_supervisor->layers.push_back(layer);

    map_script.CloseTable();
  }
  map_script.CloseTable();

  cout << "|\n--------- Collision Information ---------" << endl;
  // Load object data
  if (!object_supervisor)
    object_supervisor = new private_map_mode::ObjectSupervisor();

  cout << "| " << "Static Collision Matrix" << endl;

  map_script.OpenTable("collision");

  if (map_script.HasError())
  {
    map_script.PrintErrors();
    return false;
  }

  for (int i = 0; i < map_height; ++i)
  {
    map_script.OpenTableIntegers(i + 1);
    cout << "|\t";
    vector<int> temp;
    for (int j = 0; j < map_width; ++j)
    {
      int value = map_script.ReadData<int>(j + 1, -1);
      cout << setw(2) << value << " ";
      temp.push_back(value);
    }
    object_supervisor->collision_grid.push_back(temp);
    cout << endl;
    map_script.CloseTable();
  }
  map_script.CloseTable();

  cout << "-----------------------------------------" << endl;

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

  cout << "|\tImage Name: " << image_name << endl;
  cout << "|\tImage Path: " << image_path << endl;

  int margin = tileset_script.ReadData<int>("margin", -1);
  int spacing = tileset_script.ReadData<int>("spacing", -1);
  int num_cols = tileset_script.ReadData<int>("num_cols", -1);
  int num_rows = tileset_script.ReadData<int>("num_rows", -1);

  int tile_width = tileset_script.ReadData<int>("tile_width", -1);
  int tile_height = tileset_script.ReadData<int>("tile_height", -1);

  cout << "|\tMargin / Spacing: " << margin << " / " << spacing << endl;
  cout << "|\tSize: " << num_cols << " x " << num_rows << endl;
  cout << "|\tTile Size: " << tile_width << " x " << tile_height << endl;

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
      if (loaded_id >= 0) // Success
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
//
// sf::Vector2i MapMode::GetCameraOffset()
// {
//   sf::Vector2i position = camera->GetPosition();
//   sf::Vector2i temp;
//   temp.x = static_cast<int>(position.x) % 32;
//   temp.y = static_cast<int>(position.y) % 32;
//   return temp;
// }

void MapMode::UpdateCameraFrame()
{
  sf::Vector2i position = camera->GetPosition();
  position.x += 16;
  position.y += 16;

  // if (camera_timer.IsRunning())
  // {
  //   position.x += (1.0f - camera_timer.PercentComplete()) * delta_x;
  //   position.y += (1.0f - camera_timer.PercentComplete()) * delta_y;
  // }

  frame.tile_x_offset = static_cast<int>(position.x) % 32;
  frame.tile_y_offset = static_cast<int>(position.y) % 32;

  // The starting row and column of tiles to draw is determined by the map camera's position
  frame.tile_x_start = static_cast<signed int>(position.x / 32 - rpg_video::VideoManager->GetScreenWidth() / 64) - 1;
  frame.tile_y_start = static_cast<signed int>(position.y / 32 - rpg_video::VideoManager->GetScreenHeight() / 64) - 1;

  frame.screen_edges.left = (position.x) - rpg_video::VideoManager->GetScreenWidth() / 2;
  frame.screen_edges.right = (position.x) + rpg_video::VideoManager->GetScreenWidth() / 2;
  frame.screen_edges.top = (position.y) - rpg_video::VideoManager->GetScreenHeight() / 2;
  frame.screen_edges.bottom = (position.y) + rpg_video::VideoManager->GetScreenHeight() / 2;

    // Check for boundary conditions and re-adjust as necessary so we don't draw outside the map area

    // Usually the map centers on the camera's position, but when the camera becomes too close to
    // the edges of the map, we need to modify the drawing properties of the frame.

    // Reinit map corner check members
    // _camera_x_in_map_corner = false;
    // _camera_y_in_map_corner = false;

    // Determine the number of rows and columns of tiles that need to be drawn
    frame.num_draw_x_axis = static_cast<int>(rpg_video::VideoManager->GetScreenWidth() / 32) + 3;
    frame.num_draw_y_axis = static_cast<int>(rpg_video::VideoManager->GetScreenHeight() / 32) + 3;

    //cout << "START: (" << frame.tile_x_start << ", " << frame.tile_y_start << ")" << endl;
    //cout << "NUMBR: (" << frame.num_draw_x_axis << ", " << frame.num_draw_y_axis << ")" << endl;

    //cout << rpg_system::SystemManager->GetUpdateTime() << endl;
    // if (rpg_system::SystemManager->GetUpdateTime() == 16)
    // {
    //   cout << "===============Camera Report===============" << endl;
    //   cout << "| Position: (" << position.x << ", " << position.y << ")" << endl;
    //   cout << "| TileStart: (" << frame.tile_x_start << ", " << frame.tile_y_start << ")" << endl;
    //   cout << "| Offset: (" << frame.tile_x_offset << ", " << frame.tile_y_offset << ")" << endl;
    //   cout << "===========================================" << endl;
    // }

  // Reinit map corner check members
  // camera_x_in_map_corner = false;
  // camera_y_in_map_corner = false;

  // Determine the number of rows and columns of tiles that need to be drawn
  //frame.num_draw_x_axis = TILES_ON_X_AXIS + 1;
  //frame.num_draw_y_axis = TILES_ON_Y_AXIS + 1;

  // Camera exceeds the left boundary of the map
  // if (frame.tile_x_start <= 0) {
  //     frame.tile_x_start = 0;
  //     frame.tile_x_offset = 0;//vt_utils::FloorToFloatMultiple(1.0f, _pixel_length_x);
  //     frame.screen_edges.left = 0.0f;
  //     frame.screen_edges.right = 1024;//SCREEN_GRID_X_LENGTH;
  //     //frame.num_draw_x_axis = //TILES_ON_X_AXIS;
  //     //camera_x_in_map_corner = true;
  // }
  // Camera exceeds the right boundary of the map
  // else if(frame.tile_x_start +  >= tile_supervisor->_num_tile_on_x_axis) {
  //     frame.tile_x_start = static_cast<int16_t>(_tile_supervisor->_num_tile_on_x_axis - TILES_ON_X_AXIS);
  //     frame.tile_x_offset = vt_utils::FloorToFloatMultiple(1.0f, _pixel_length_x);
  //     frame.screen_edges.right = static_cast<float>(_object_supervisor->_num_grid_x_axis);
  //     frame.screen_edges.left = _map_frame.screen_edges.right - SCREEN_GRID_X_LENGTH;
  //     frame.num_draw_x_axis = TILES_ON_X_AXIS;
  //     camera_x_in_map_corner = true;
  // }

  // Camera exceeds the top boundary of the map
  // if (frame.tile_y_start <= 0) {
  //     frame.tile_y_start = 0;
  //     frame.tile_y_offset = 0;//vt_utils::FloorToFloatMultiple(2.0f, _pixel_length_y);
  //     frame.screen_edges.top = 0.0f;
  //     frame.screen_edges.bottom = 768;//SCREEN_GRID_Y_LENGTH;
  //     //_map_frame.num_draw_y_axis = TILES_ON_Y_AXIS;
  //     //camera_y_in_map_corner = true;
  // }
  // Camera exceeds the bottom boundary of the map
  // else if(_map_frame.tile_y_start + TILES_ON_Y_AXIS >= _tile_supervisor->_num_tile_on_y_axis) {
  //     _map_frame.tile_y_start = static_cast<int16_t>(_tile_supervisor->_num_tile_on_y_axis - TILES_ON_Y_AXIS);
  //     _map_frame.tile_y_offset = vt_utils::FloorToFloatMultiple(2.0f, _pixel_length_y);
  //     _map_frame.screen_edges.bottom = static_cast<float>(_object_supervisor->_num_grid_y_axis);
  //     _map_frame.screen_edges.top = _map_frame.screen_edges.bottom - SCREEN_GRID_Y_LENGTH;
  //     _map_frame.num_draw_y_axis = TILES_ON_Y_AXIS;
  //     _camera_y_in_map_corner = true;
  // }
  //
  // Update parallax effects now that map corner members are up to date
  // if (camera_timer.IsRunning())
  // {
  //   // Inform the effect supervisor about camera movement.
  //   float duration = (float)camera_timer.GetDuration();
  //   float time_elapsed = (float)rpg_system::SystemManager->GetUpdateTime();
  //   // float x_parallax = !_camera_x_in_map_corner ?
  //   //                    _delta_x * time_elapsed / duration
  //   //                    / SCREEN_GRID_X_LENGTH * VIDEO_STANDARD_RES_WIDTH :
  //   //                    0.0f;
  //   // float y_parallax = !_camera_y_in_map_corner ?
  //   //                    _delta_y * time_elapsed / duration
  //   //                    / SCREEN_GRID_Y_LENGTH * VIDEO_STANDARD_RES_HEIGHT :
  //   //                    0.0f;
  //
  //   //GetEffectSupervisor().AddParallax(x_parallax, y_parallax);
  //   //GetIndicatorSupervisor().AddParallax(x_parallax, y_parallax);
  // }

  // frame.tile_x_start = static_cast<int>(position.x / 32) - rpg_video::VideoManager->GetScreenWidth() / 64;
  // //if (frame.tile_x_start < 0)
  //   //frame.tile_x_start = 0;
  // frame.tile_y_start = static_cast<int>(position.y / 32) - rpg_video::VideoManager->GetScreenHeight() / 64;
  //
  // frame.tile_x_offset = static_cast<int>(position.x) % 32;
  // frame.tile_y_offset = static_cast<int>(position.y) % 32;
  //
  // frame.num_draw_x_axis = static_cast<int>(rpg_video::VideoManager->GetScreenWidth() / 32) + 2;
  // frame.num_draw_y_axis = static_cast<int>(rpg_video::VideoManager->GetScreenHeight() / 32) + 2;
  //
  // frame.screen_edges.left = (position.x) - rpg_video::VideoManager->GetScreenWidth() / 32;
  // frame.screen_edges.right = (position.x) + rpg_video::VideoManager->GetScreenWidth() / 32;
  // frame.screen_edges.top = (position.y) - rpg_video::VideoManager->GetScreenHeight() / 32;
  // frame.screen_edges.bottom = (position.y) + rpg_video::VideoManager->GetScreenHeight() / 32;

  //cout << "LEFT: " << (position.x) - rpg_video::VideoManager->GetScreenWidth() / 2 << endl;
}

// sf::IntRect MapMode::GetCameraBounds()
// {
//   // if (tile_dimensions.x == 0 || tile_dimensions.y == 0)
//   //   return sf::IntRect(0, 0, 0, 0);
//   sf::Vector2i position = camera->GetPosition();
//
//   int tile_x = static_cast<int>(position.x / 32) - rpg_video::VideoManager->GetScreenWidth() / 64;
//   int tile_y = static_cast<int>(position.y / 32) - rpg_video::VideoManager->GetScreenHeight() / 64;
//
//   /// If the whole size of the camera is not divisible by the tile
//   /// size, we need to add one more tile to the bounds.
//   /// We also have to add one to the numbers to make the tile count
//   /// begin at 1 as opposed to 0.
//   int tiles_across = static_cast<int>(rpg_video::VideoManager->GetScreenWidth() / 32) + 2;
//   int tiles_down = static_cast<int>(rpg_video::VideoManager->GetScreenHeight() / 32) + 2;
//
//   /// If we are not perfectly positioned over the tiles, we need to
//   /// add one more tile to the frame to make sure no black is shown.
//   if (static_cast<int>(position.x) % 32 != 0)
//     ++tiles_across;
//   if (static_cast<int>(position.y) % 32 != 0)
//     ++tiles_down;
//
//   return sf::IntRect(tile_x, tile_y, tiles_across, tiles_down);
// }

}
