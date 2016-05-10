#include "../../utils/globals.hpp"
#include "map_objects.hpp"

#include "map_mode.hpp"

namespace rpg_map_mode
{

namespace private_map_mode
{

MapObject::MapObject(MapObjectDrawLayer _layer)
  : object_id(0)
  , tile_position(0, 0)
  , draw_layer(_layer)
  , visible(true)
{}

bool MapObject::ShouldDraw()
{
  if (!visible)
    return false;

  MapMode* map_mode = MapMode::CurrentInstance();
  //cout << "MAP " << map_mode->GetWindowFrame().screen_edges.left << " :: " << map_mode->GetWindowFrame().screen_edges.right << endl;
  MapRectangle grid_rect(tile_position.x, tile_position.y, tile_position.x + 32, tile_position.y + 32);
  if (!MapRectangle::CheckIntersection(grid_rect, map_mode->GetWindowFrame().screen_edges))
    return false;

  return true;
}

MapRectangle MapObject::GetGridCollisionRectangle() const
{
  MapRectangle rect;
  rect.left = tile_position.x;
  rect.top = tile_position.y;
  rect.right = tile_position.x + 32;
  rect.bottom = tile_position.y + 32;
  return rect;
}

MapRectangle MapObject::GetGridCollisionRectangle(float tile_x, float tile_y) const
{
  MapRectangle rect;
  rect.left = tile_x;
  rect.right = tile_x + 32;
  rect.top = tile_y;
  rect.bottom = tile_y + 32;
  return rect;
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
  , visible_hero(nullptr)
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
  cout << "ID_COUNT: " << id_counter << endl;
  int temp = 0;
  if (reusable_ids.empty())
    temp = id_counter++;
  else
  {
    temp = reusable_ids.front();
    reusable_ids.pop();
  }
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

CollisionType ObjectSupervisor::DetectCollision(MapObject* _object, const float _x, const float _y, MapObject* _collision_object)
{
  // If the sprite has this property set it can not collide
  if(!_object)
      return NO_COLLISION;

  // Get the collision rectangle at the given position
  MapRectangle sprite_rect = _object->GetGridCollisionRectangle(_x, _y);

  // Check if any part of the object's collision rectangle is outside of the map boundary
  if(sprite_rect.left < 0.0f || sprite_rect.right >= static_cast<float>(collision_grid.size() * 32) ||
          sprite_rect.top < 0.0f || sprite_rect.bottom >= static_cast<float>(collision_grid[0].size() * 32)) {
      return WALL_COLLISION;
  }

  // Check for the absence of collision checking after the map boundaries check,
  // So that no collision beings won't get out of the map.
  // if(object->GetCollisionMask() == NO_COLLISION)
  //     return NO_COLLISION;

  // Check if the object's collision rectangle overlaps with any unwalkable elements on the collision grid
  // Grid based collision is not done for objects in the sky layer
  if(_object->GetObjectDrawLayer() != rpg_map_mode::SKY_OBJECT) {//&& object->GetCollisionMask() & WALL_COLLISION) {
      // Determine if the object's collision rectangle overlaps any unwalkable tiles
      // Note that because the sprite's collision rectangle was previously determined to be within the map bounds,
      // the map grid tile indeces referenced in this loop are all valid entries and do not need to be checked for out-of-bounds conditions
      for(int y = static_cast<int>(sprite_rect.top); y <= static_cast<int>(sprite_rect.bottom); ++y) {
          for(int x = static_cast<int>(sprite_rect.left); x <= static_cast<int>(sprite_rect.right); ++x) {
              // Checks the collision grid at the row-column at the object's current context
              if(collision_grid[y / 32][x / 32] > 0)
                  return WALL_COLLISION;
          }
      }
  }

  //std::vector<MapObject *>* objects = &_GetObjectsFromDrawLayer(object->GetObjectDrawLayer());

  // std::vector<vt_map::private_map::MapObject *>::const_iterator it, it_end;
  // for(it = objects->begin(), it_end = objects->end(); it != it_end; ++it) {
  //     MapObject *collision_object = *it;
  //     // Check if the object exists and has the no_collision property enabled
  //     if(!collision_object || collision_object->GetCollisionMask() == NO_COLLISION)
  //         continue;
  //
  //     // Object and sprite are the same
  //     if(collision_object->GetObjectID() == object->GetObjectID())
  //         continue;
  //
  //     // If the two objects aren't colliding, try next.
  //     if(!CheckObjectCollision(sprite_rect, collision_object))
  //         continue;
  //
  //     // The two objects are colliding, return the potentially asked pointer to it.
  //     if(collision_object_ptr != nullptr)
  //         *collision_object_ptr = collision_object;
  //
  //     // When the collision mask is taking in account the collision type
  //     // we can return it. Otherwise, just ignore the sprite colliding.
  //     COLLISION_TYPE collision = GetCollisionFromObjectType(collision_object);
  //     if(object->GetCollisionMask() & collision)
  //         return collision;
  //     else
  //         continue;
  // }

  return NO_COLLISION;
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
