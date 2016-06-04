#include "../../utils/globals.hpp"
#include "map_tiles.hpp"

#include "../../core/global/global.hpp"
#include "../../core/video_manager.hpp"
#include "../../core/image.hpp"
#include "map_mode.hpp"

namespace rpg_map_mode
{

namespace private_map_mode
{

TileSupervisor::~TileSupervisor()
{
  // for (int i = 0; i < tile_images.size(); ++i)
  //   delete tile_images[i];
  //
  // tile_images.clear();
}

// bool TileSupervisor::Load(sel::Selector& _map_data)
// {
//   auto map_data = _map_data;
//
//   vector<string> tileset_filenames;
//   int num_tilesets = map_data["tileset_filenames"]["count"];
//   for (int i = 0; i < num_tilesets; ++i)
//     tileset_filenames.push_back(map_data["tileset_filenames"][i + 1]);
//
//   // Load in the tileset data
//   for (int i = 0; i < num_tilesets; ++i)
//   {
//     IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "Loading tileset: " << tileset_filenames[i] << endl;
//
//     State tileset_state(true);
//     if (!tileset_state.Load(tileset_filenames[i]))
//     {
//       PRINT_ERROR << "Failed to load tileset Lua: " << tileset_filenames[i] << endl;
//       return false;
//     }
//
//     auto data = tileset_state["tileset_data"];
//
//     // Import all of the data points
//     string tileset_name = data["name"];
//     string image_path = data["image_path"];
//     int margin = int(data["margin"]);
//     int spacing = int(data["spacing"]);
//     int num_cols = int(data["num_cols"]);
//     int num_rows = int(data["num_rows"]);
//     int tile_width = int(data["tile_width"]);
//     int tile_height = int(data["tile_height"]);
//
//     IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "Tileset: " << tileset_name << endl;
//     IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "Loading tileset image: " << image_path << endl;
//
//     sf::Image temp_image;
//     if (!temp_image.loadFromFile(image_path))
//     {
//       PRINT_ERROR << "Failed to load image: " << image_path << endl;
//       return false;
//     }
//
//     for (int row = 0; row < num_rows; ++row)
//     {
//       for (int col = 0; col < num_cols; ++col)
//       {
//         tile_images.push_back(new sf::Texture());
//
//         sf::IntRect tile_rect(margin + spacing * col + col * tile_width,
//                               margin + spacing * row + row * tile_height,
//                               32, 32);
//
//         if (!tile_images.back()->loadFromImage(temp_image, tile_rect))
//         {
//           PRINT_ERROR << "Failed to load tile from image" << endl;
//           delete tile_images.back();
//           tile_images.pop_back();
//           return false;
//         }
//       }
//     }
//   }
//
//   // Load in the layer data
//   int map_width = int(map_data["num_cols"]);
//   int map_height = int(map_data["num_rows"]);
//
//   auto layers_data = map_data["layers"];
//   int num_layers = int(layers_data["num_layers"]);
// cout << "Num: " << num_layers << endl;
//   for (int l = 0; l < num_layers; ++l)
//   {
//     MapLayerType type;
//     string str_type = layers_data[l]["type"];
//
//     if (str_type == "ground")
//       type = MapLayerType::GROUND;
//     else if (str_type == "wall")
//       type = MapLayerType::WALL;
//     else if (str_type == "sky")
//       type = MapLayerType::SKY;
//     else
//     {
//       PRINT_ERROR << "Unknown type for layer" << endl;
//       return false;
//     }
//
//     MapLayer layer(type);
//     for (int row = 0; row < map_height; ++row)
//     {
//       vector<int> temp;
//       for (int col = 0; col < map_width; ++col)
//       {
//         temp.push_back(layers_data[l][row + 1][col + 1]);
//       }
//       layer.tiles.push_back(temp);
//     }
//     layers.push_back(layer);
//   }
//
//   return true;
// }

void TileSupervisor::DrawLayers(const MapFrame& _frame, const MapLayerType& _layer_type)
{
  int tile_y_end = _frame.tile_y_start + _frame.num_draw_y_axis;
  int tile_x_end = _frame.tile_x_start + _frame.num_draw_x_axis;

  // cout << "TILES: " << _frame.tile_x_offset << " :: " << _frame.tile_y_offset << endl;

  //cout << _frame.num_draw_x_axis << endl;

  for (int l = 0; l < layers.size(); ++l)
  {
    if (layers[l].layer_type != _layer_type)
      continue;

    for (int y = 0, row = _frame.tile_y_start; row < tile_y_end; ++y, ++row)
      for (int x = 0, col = _frame.tile_x_start; col < tile_x_end; ++x, ++col)
      {
        if (row >= layers[l].tiles.size() || row < 0 ||
            col >= layers[l].tiles[row].size() || col < 0)
          continue;

        int tile_gid = layers[l].tiles[row][col];

        if (tile_gid == -1)
          continue;

        sf::Vector2f pos(x * 32 - _frame.tile_x_offset, y * 32 - _frame.tile_y_offset);
        rpg_resource::ResourceManager
            ->GetImage(tile_ids[tile_gid])
            ->SetPosition(pos.x, pos.y);

        rpg_resource::ResourceManager->DrawImage(tile_ids[tile_gid]);
      }
  }
}

}

}
