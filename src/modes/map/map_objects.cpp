#include "../../utils/globals.hpp"
#include "map_objects.hpp"

#include "map_mode.hpp"

namespace rpg_map_mode
{

namespace private_map_mode
{

MapObject::MapObject(MapObjectDrawLayer _layer)
  : draw_layer(_layer)
  , object_id(0)
  , tile_position(0, 0)
  , visible(false)
{}

MapRectangle MapObject::GetGridCollisionRectangle() const
{
  MapRectangle rect;
  rect.left = tile_position.x;
}

void MapObject::SetPosition(const float _x, const float _y)
{
  tile_position.x = _x;
  tile_position.y = _y;
}

void MapObject::Move(const float _x, const float _y)
{
  SetPosition(tile_position.x + _x, tile_position.y + _y);
}

bool MapObject::IsVisible()
{
  return visible;
}

void MapObject::SetVisible(const bool _visible)
{
  visible = _visible;
}

unsigned int MapObject::GetObjectId() const
{
  return object_id;
}

sf::Vector2i MapObject::GetPosition() const
{
  return tile_position;
}

/***************************************
 * Object Supervisor Class Definitions *
 ***************************************/
ObjectSupervisor::ObjectSupervisor()
  : id_counter(0)
{
  IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "ObjectSupervisor constructor called" << endl;
}

ObjectSupervisor::~ObjectSupervisor()
{
  IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "ObjectSupervisor destructor called" << endl;

  // TODO: Need to delete MapObjects*
}

unsigned int ObjectSupervisor::GenerateObjectId()
{
  int temp = 0;
  if (reusable_ids.empty())
    temp = id_counter++;

  temp = reusable_ids.front();
  reusable_ids.pop();
  return temp;
}

MapObject* ObjectSupervisor::GetObject(const unsigned int _id)
{
  if (_id >= all_objects.size() || !all_objects[_id])
  {
    IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "Requested object id invalid" << endl;
    return nullptr;
  }

  return all_objects[_id];
}

void ObjectSupervisor::RegisterObject(MapObject* _object)
{
  if (!_object)
    return;

  int id = GenerateObjectId();

  all_objects[id] = _object;
}

void ObjectSupervisor::DeleteObject(const unsigned int _id)
{
  if (_id >= all_objects.size() || !all_objects[_id])
    return;

  delete all_objects[_id];
  all_objects[_id] = nullptr;
  reusable_ids.push(_id);
}

void ObjectSupervisor::SortObjects()
{

}

bool ObjectSupervisor::IsMapCollision(const unsigned int _x, const unsigned int _y)
{
  return collision_grid[_y][_x];
}

void ObjectSupervisor::Update()
{

}

void ObjectSupervisor::DrawObjects()
{
  map<unsigned int, MapObject*>::iterator iter = all_objects.begin();
  for (; iter != all_objects.end(); iter++)
    iter->second->Draw();
}



}

}
