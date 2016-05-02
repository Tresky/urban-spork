#include "../utils/globals.hpp"
#include "script.hpp"

#include "resources.hpp"

#include "../modes/map/map_mode.hpp"
#include "../modes/map/map_tiles.hpp"
#include "../modes/map/map_objects.hpp"
using namespace rpg_map_mode;
using namespace private_map_mode;

#include "../utils/lua_bridge.hpp"
using namespace luabridge;

namespace rpg_script
{

ScriptEngine* ScriptManager = nullptr;
bool SCRIPT_DEBUG = true;

/**********************************
 * ScriptEngine Class Definitions *
 **********************************/
ScriptEngine::ScriptEngine()
  : L(luaL_newstate())
{
  luaL_openlibs(L);
  IF_PRINT_DEBUG(SCRIPT_DEBUG) << "ScriptEngine constructor called" << endl;
}

ScriptEngine::~ScriptEngine()
{
  IF_PRINT_DEBUG(SCRIPT_DEBUG) << "ScriptEngine destructor called" << endl;

  // map<string, Script*>::iterator iter = script.begin();
  // for (; iter != scripts.end(); ++iter)
  //   delete iter;

  if (L)
    lua_close(L);
}

bool ScriptEngine::InitSingleton()
{
  return true;
}

// bool ScriptEngine::Load(string& _lua_filepath, ScriptType _type)
// {
//   switch (_type)
//   {
//     case SCRIPT_TYPE_CHARACTER:
//       return LoadCharacter(_lua_filepath);
//     break;
//     case SCRIPT_TYPE_TILEMAP:
//       return LoadTilemap(_lua_filepath);
//     break;
//     case SCRIPT_TYPE_TILESET:
//       return LoadTileset(_lua_filepath);
//     break;
//     default:
//       IF_PRINT_WARNING(SCRIPT_DEBUG) << "Invalid script type" << endl;
//       return false;
//     break;
//   }
// }

bool ScriptEngine::LoadGlobalCharacter(string& _lua_filepath)
{
  // GlobalCharacter temp_char;
  //
  // int id = ResourceEngine->LoadResource("data/gchar/hero.png"))
  // if (id < 0)
  //   return false;
  //
  // temp_char.resource_id = id;

  return true;
}

bool ScriptEngine::LoadTilemap(string& _lua_filepath, rpg_map_mode::MapMode* _map)
{
  cout << "---------- Tilemap Information ----------" << endl;
  cout << "| File: " << _lua_filepath << endl;

  if (!_map)
  {
    PRINT_ERROR << "MapMode is null" << endl;
    return false;
  }

  // These return 0 on success, so the logic is backwards
  if (luaL_loadfile(L, _lua_filepath.c_str()) || lua_pcall(L, 0, 0, 0))
  {
    PRINT_ERROR << "Script failed to load: " << _lua_filepath << endl;
    return false;
  }

  LuaRef map_data = getGlobal(L, "map_data");
  unsigned int map_width = map_data["num_cols"].cast<int>();
  unsigned int map_height = map_data["num_rows"].cast<int>();

  cout << "| Name: " << map_data["name"].cast<string>() << endl;
  cout << "|" << endl;

  cout << "| Dimensions: " << map_width << " x " << map_height << endl;
  cout << "|\n---------- Tileset Information ----------" << endl;

  // Temporary tile supervisor to store data
  // until loading is complete.
  _map->tile_supervisor = new TileSupervisor();

  // Load in the tileset data
  LuaRef tilesets_data = map_data["tilesets"];
  int num_tilesets = tilesets_data["count"].cast<int>();
  cout << "| Tileset Count: " << num_tilesets << endl;
  for (int i = 0; i < num_tilesets; ++i)
  {
    string path = tilesets_data[i].cast<string>();
    cout << "| " << i << ". " << path << endl;
    if (!LoadTileset(path, _map->tile_supervisor))
    {
      PRINT_ERROR << "Failed to load tileset" << endl;

      // Clean up
      delete _map->tile_supervisor;
      _map->tile_supervisor = nullptr;
      return false;
    }
  }
  cout << "|\n----------- Layer Information -----------" << endl;

  // Load in the layer data
  LuaRef layers_data = map_data["layers"];
  int num_layers = layers_data["num_layers"].cast<int>();
  cout << "| Layer Count: " << num_layers << endl;
  for (int l = 0; l < num_layers; ++l)
  {
    MapLayerType type;
    LuaRef curr_layer = layers_data[l];
    string str_type = curr_layer["type"];

    if (str_type == "ground")
      type = MapLayerType::GROUND;
    else if (str_type == "wall")
      type = MapLayerType::WALL;
    else if (str_type == "sky")
      type = MapLayerType::SKY;
    else
    {
      PRINT_ERROR << "Unknown type for layer" << endl;
      continue;
      // // Clean up
      // delete _map->tile_supervisor;
      // _map->tile_supervisor = nullptr;
      // return false;
    }

    cout << "| " << l << ". " << str_type << endl;

    MapLayer layer(type);
    for (int row = 0; row < map_height; ++row)
    {
      cout << "|\t";
      LuaRef curr_row = curr_layer[row + 1];
      vector<int> temp;
      for (int col = 0; col < map_width; ++col)
      {
        int id = curr_row[col + 1].cast<int>();
        cout << setw(3) << id << " ";
        temp.push_back(id);
      }
      cout << endl;
      layer.tiles.push_back(temp);
    }

    _map->tile_supervisor->layers.push_back(layer);
  }

  cout << "|\n--------- Collision Information ---------" << endl;
  // Load object data
  _map->object_supervisor = new ObjectSupervisor();

  cout << "| " << "Static Collision Matrix" << endl;

  LuaRef collisions = map_data["collision"];
  for (int i = 0; i < map_height; ++i)
  {
    cout << "|\t";
    vector<int> temp;
    LuaRef row_data = collisions[i + 1];
    for (int j = 0; j < map_width; ++j)
    {
      int value = row_data[j + 1].cast<int>();
      cout << setw(2) << value << " ";
      temp.push_back(value);
    }
    _map->object_supervisor->collision_grid.push_back(temp);
    cout << endl;
  }

  cout << "-----------------------------------------" << endl;

  return true;
}

bool ScriptEngine::LoadTileset(const string& _lua_filepath, rpg_map_mode::private_map_mode::TileSupervisor* _ts)
{
  if (!_ts)
  {
    PRINT_ERROR << "TileSupervisor is null" << endl;
    return false;
  }

  // These return 0 on success, so the logic is backwards
  if (luaL_loadfile(L, _lua_filepath.c_str()) || lua_pcall(L, 0, 0, 0))
  {
    PRINT_ERROR << "Script failed to load: " << _lua_filepath << endl;
    return false;
  }

  LuaRef tileset_data = getGlobal(L, "tileset_data");
  string image_name = tileset_data["name"].cast<string>();
  string image_path = tileset_data["image_path"].cast<string>();

  cout << "|\tImage Name: " << image_name << endl;
  cout << "|\tImage Path: " << image_path << endl;

  int margin = tileset_data["margin"].cast<int>();
  int spacing = tileset_data["spacing"].cast<int>();
  int num_cols = tileset_data["num_cols"].cast<int>();
  int num_rows = tileset_data["num_rows"].cast<int>();

  int tile_width = tileset_data["tile_width"].cast<int>();
  int tile_height = tileset_data["tile_height"].cast<int>();

  cout << "|\tMargin / Spacing: " << margin << " / " << spacing << endl;
  cout << "|\tSize: " << num_cols << " x " << num_rows << endl;
  cout << "|\tTile Size: " << tile_width << " x " << tile_height << endl;

  for (int row = 0; row < num_rows; ++row)
  {
    for (int col = 0; col < num_cols; ++col)
    {
      sf::IntRect tile_rect(margin + spacing * col + col * tile_width,
                            margin + spacing * row + row * tile_height,
                            32, 32);

      int loaded_id = rpg_resource::ResourceManager->LoadImageRect(image_path, tile_rect);
      if (loaded_id >= 0) // Success
        _ts->tile_ids.push_back(loaded_id);
      else
      {
        PRINT_ERROR << "Failed to load tile from image" << endl;
        return false;
      }
    }
  }
  return true;
}

}
