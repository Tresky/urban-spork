#ifndef MAP_TILES_HPP
#define MAP_TILES_HPP

#include "map_utils.hpp"

namespace rpg_map_mode
{
class MapMode;

namespace private_map_mode
{

//
enum MapLayerType
{
  INVALID = -1,
  COLLISION = 0,
  GROUND = 1,
  WALL = 2,
  SKY = 3
};

/**
 *
 */
class MapLayer
{
public:
  /**
   * Constructor
   */
  MapLayer(MapLayerType _layer_type)
    : layer_type(_layer_type)
  {}

  // 2-Dimensional vector tile GIDs
  std::vector< std::vector<int> > tiles;

  // Enumerator with the map type.
  // GROUND - Backgrond tiles with walkability
  // WALL - Tiles with no walkability
  // SKY - Tiles that pass over the head entities
  MapLayerType layer_type;
};

/**
 *
 */
class TileSupervisor
{
  //friend class rpg_map_mode::MapMode;
  friend class rpg_map_mode::MapMode;
public:
  /**
   * Default Constructor
   */
  TileSupervisor()
    : collision_layer(MapLayerType::COLLISION)
  {}

  /**
   * Destructor
   */
  ~TileSupervisor();

  //bool Load(sel::Selector& _map_data);

  void Update(){} // For updating animated tiles

  bool IsTileWalkable(const signed int _x, const signed int _y)
  {
    return !collision_layer.tiles[_y][_x];
  }

  // int GetWidthHeight()
  // {
  //   return layers.begin()->size();
  // }

  void DrawLayers(const MapFrame& _frame, const MapLayerType& _layer_type);

private:
  std::vector<MapLayer> layers;

  MapLayer collision_layer;

  std::vector<int> tile_ids;
};

}

}

#endif // MAP_TILES_HPP
