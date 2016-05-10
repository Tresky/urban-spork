#ifndef MAP_OBJECTS_HPP
#define MAP_OBJECTS_HPP

#include <queue>
#include <SFML/Graphics.hpp>

#include "map_utils.hpp"

namespace rpg_map_mode
{
class MapMode;

enum MapObjectDrawLayer
{
  NO_LAYER_OBJECT = -1,
  GROUND_OBJECT = 0,
  WALL_OBJECT = 1,
  SKY_OBJECT = 2,
  PASS_OBJECT = 3
};

namespace private_map_mode
{
class MapSprite;

class MapObject
{
public:
  MapObject(MapObjectDrawLayer _layer);

  virtual ~MapObject()
  {}

  bool ShouldDraw();

  MapRectangle GetGridCollisionRectangle() const;

  MapRectangle GetGridCollisionRectangle(float tile_x, float tile_y) const;

  MapObjectDrawLayer GetObjectDrawLayer() const
  {
    return draw_layer;
  }

  void SetPosition(const float _x, const float _y);

  void Move(const float _x, const float _y);

  bool IsVisible();

  void SetVisible(bool _visible);

  unsigned int GetObjectId() const;

  sf::Vector2i GetPosition() const;

  virtual void Update() = 0;
  virtual void Draw() = 0;

  //bool ShouldDraw();


protected:
  unsigned int object_id;

  sf::Vector2i tile_position;

  bool visible;

  MapObjectDrawLayer draw_layer;

};

class ObjectSupervisor
{
  friend class rpg_map_mode::MapMode;
public:
  ObjectSupervisor();

  ~ObjectSupervisor();

  void SetPartyMemberSprite(private_map_mode::MapSprite* _visible_hero)
  {
    visible_hero = _visible_hero;
  }

  unsigned int GenerateObjectId();

  MapObject* GetObject(const unsigned int _id);

  void RegisterObject(MapObject* _object);

  void DeleteObject(const unsigned int _id);

  void SortObjects();

  bool IsMapCollision(const unsigned int _x, const unsigned int _y);

  CollisionType DetectCollision(MapObject* _object, const float _x, const float _y, MapObject* _collision_object);

  void Update();

  void DrawObjects();
private:

  unsigned int id_counter;

  std::queue<int> reusable_ids;

  std::vector< std::vector<int> > collision_grid;

  private_map_mode::MapSprite* visible_hero;

  std::map<unsigned int, MapObject*> all_objects;

  // std::vector<MapObject*> ground_objects;
  //
  // std::vector<MapObject*> wall_objects;
  //
  // std::vector<MapObject*> sky_objects;
};

}

}

#endif // MAP_OBJECTS_HPP
