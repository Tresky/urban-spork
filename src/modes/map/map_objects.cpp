#include "../../utils/globals.hpp"
#include "map_objects.hpp"

#include "map_mode.hpp"

namespace rpg_map_mode
{

namespace private_map_mode
{

MapObject::MapObject(MapObjectDrawLayer _layer)
  : object_id(-1)
  , tile_position(0, 0)
  , dimensions(0, 0)
  , visible(true)
  , draw_layer(_layer)
  , collision_mask(ALL_COLLISION)
  , object_type(OBJECT_TYPE)
{
  ObjectSupervisor* obj_sup = MapMode::CurrentInstance()->GetObjectSupervisor();
  object_id = obj_sup->GenerateObjectId();
  obj_sup->RegisterObject(this);
}

bool MapObject::ShouldDraw()
{
  if (!visible)
    return false;

  MapMode* map_mode = MapMode::CurrentInstance();
  //cout << "MAP " << map_mode->GetWindowFrame().screen_edges.left << " :: " << map_mode->GetWindowFrame().screen_edges.right << endl;
  MapRectangle grid_rect(tile_position.x, tile_position.y, tile_position.x + dimensions.x, tile_position.y + dimensions.y);
  if (!MapRectangle::CheckIntersection(grid_rect, map_mode->GetWindowFrame().screen_edges))
    return false;

  return true;
}

MapRectangle MapObject::GetGridCollisionRectangle() const
{
  MapRectangle rect;
  rect.left = tile_position.x;
  rect.top = tile_position.y;
  rect.right = tile_position.x + dimensions.x;
  rect.bottom = tile_position.y + dimensions.y;
  return rect;
}

MapRectangle MapObject::GetGridCollisionRectangle(float tile_x, float tile_y) const
{
  MapRectangle rect;
  rect.left = tile_x;
  rect.right = tile_x + dimensions.x;
  rect.top = tile_y;
  rect.bottom = tile_y + dimensions.y;
  return rect;
}

void MapObject::SetDimensions(const int _x, const int _y)
{
  dimensions = sf::Vector2i(_x, _y);
}

sf::Vector2i MapObject::GetDimensions() const
{
  return dimensions;
}

void MapObject::SetPosition(const int _x, const int _y)
{
  tile_position.x = _x;
  tile_position.y = _y;
}

void MapObject::SetCenterPosition(const int _x, const int _y)
{
  tile_position.x = _x - dimensions.x / 2;
  tile_position.y = _y - dimensions.y / 2;
}

void MapObject::Move(const int _x, const int _y)
{
  SetPosition(tile_position.x + _x, tile_position.y + _y);
}

void MapObject::MoveCenter(const int _x, const int _y)
{
  SetPosition(tile_position.x + _x - dimensions.x,
              tile_position.y + _y - dimensions.y);
}

bool MapObject::IsVisible()
{
  return visible;
}

void MapObject::SetVisible(const bool _visible)
{
  visible = _visible;
}

int MapObject::GetObjectId() const
{
  return object_id;
}

sf::Vector2i MapObject::GetPosition() const
{
  return tile_position;
}

sf::Vector2i MapObject::GetCenterPosition() const
{
  return sf::Vector2i(tile_position.x + dimensions.x / 2,
                      tile_position.y + dimensions.y / 2);
}

int MapObject::GetXPosition() const
{
  return tile_position.x;
}

int MapObject::GetYPosition() const
{
  return tile_position.y;
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
  int temp = 0;
  if (reusable_ids.empty())
    temp = all_objects.size();
  else
  {
    temp = reusable_ids.front();
    reusable_ids.pop();
  }

  return temp;
}

MapObject* ObjectSupervisor::GetObject(const unsigned int _id)
{
  if (_id >= all_objects.size() ||
      !all_objects[_id])
  {
    IF_PRINT_DEBUG(MAP_MODE_DEBUG) << "Requested object id invalid" << endl;
    return nullptr;
  }

  return all_objects[_id];
}

void ObjectSupervisor::RegisterObject(MapObject* _object)
{
  if (!_object ||
      _object->GetObjectId() < 0 ||
      _object->GetObjectId() > all_objects.size())
    return;

  int id = _object->GetObjectId();

  if (id == all_objects.size())
    all_objects.push_back(_object);
  else
    all_objects[id] = _object;

  switch (_object->GetObjectDrawLayer())
  {
    case GROUND_OBJECT:
      ground_objects.push_back(_object);
      break;
    case WALL_OBJECT:
    case SKY_OBJECT:
    case PASS_OBJECT:
    case NO_LAYER_OBJECT:
    default:
      break;
  }
}

void ObjectSupervisor::DeleteObject(const unsigned int _id)
{
  if (_id >= all_objects.size() ||
      !all_objects[_id])
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

bool ObjectSupervisor::CheckObjectCollision(const MapRectangle& _rect, const MapObject* _object)
{
  if(!_object)
    return false;

  MapRectangle obj_rect = _object->GetGridCollisionRectangle();
  return MapRectangle::CheckIntersection(_rect, obj_rect);
}

CollisionType ObjectSupervisor::DetectCollision(MapObject* _object, const float _x, const float _y, MapObject** _collision_object)
{
  // If the sprite has this property set it can not collide
  if(!_object)
    return NO_COLLISION;

  // Get the collision rectangle at the given position
  MapRectangle sprite_rect = _object->GetGridCollisionRectangle(_x, _y);

  // Check if any part of the object's collision rectangle is outside of the map boundary
  if (sprite_rect.top < 0.0f || sprite_rect.bottom >= static_cast<float>(collision_grid.size() * 32) ||
      sprite_rect.left < 0.0f || sprite_rect.right >= static_cast<float>(collision_grid[0].size() * 32))
    return WALL_COLLISION;

  // Check for the absence of collision checking after the map boundaries check,
  // So that no collision beings won't get out of the map.
  if (_object->GetCollisionMask() == NO_COLLISION)
    return NO_COLLISION;

  // Check if the object's collision rectangle overlaps with any unwalkable elements on the collision grid
  // Grid based collision is not done for objects in the sky layer
  if (_object->GetObjectDrawLayer() != rpg_map_mode::SKY_OBJECT && _object->GetCollisionMask() & WALL_COLLISION) {
    // Determine if the object's collision rectangle overlaps any unwalkable tiles
    // Note that because the sprite's collision rectangle was previously determined to be within the map bounds,
    // the map grid tile indeces referenced in this loop are all valid entries and do not need to be checked for out-of-bounds conditions
    for(int y = static_cast<int>(sprite_rect.top); y <= static_cast<int>(sprite_rect.bottom); ++y) {
      for(int x = static_cast<int>(sprite_rect.left); x <= static_cast<int>(sprite_rect.right); ++x) {
        // Checks the collision grid at the row-column at the object's current context
        if(collision_grid[y / 32 - 1][x / 32 - 1] > 0)
          return WALL_COLLISION;
      }
    }
  }

  // TPNOTE: Make vector selection dynamic for objects
  std::vector<MapObject*> objects = ground_objects;//_GetObjectsFromDrawLayer(object->GetObjectDrawLayer());

  for (auto collision_object : objects)
  {
    // Check if the object exists and has the no_collision property enabled
    if (!collision_object || collision_object->GetCollisionMask() == NO_COLLISION)
      continue;

    // Object and sprite are the same
    if (collision_object->GetObjectId() == _object->GetObjectId())
      continue;

    // If the two objects aren't colliding, try next.
    if (!CheckObjectCollision(sprite_rect, collision_object))
      continue;

    // The two objects are colliding, return the potentially asked pointer to it.
    if (collision_object != nullptr)
    {
      *_collision_object = collision_object;
    }

    // When the collision mask is taking in account the collision type
    // we can return it. Otherwise, just ignore the sprite colliding.
    CollisionType collision = GetCollisionFromObjectType(collision_object);
    if (_object->GetCollisionMask() & collision)
      return collision;
    else
      continue;
  }

  return NO_COLLISION;
}

CollisionType ObjectSupervisor::GetCollisionFromObjectType(MapObject *_object) const
{
  if(!_object)
    return NO_COLLISION;

  switch(_object->GetType()) {
    case VIRTUAL_TYPE:
    case SPRITE_TYPE:
      return CHARACTER_COLLISION;
      break;
    case ENEMY_TYPE:
      return ENEMY_COLLISION;
      break;
    //case TREASURE_TYPE:
    case PHYSICAL_TYPE:
      return WALL_COLLISION;
      break;
    default:
      break;
  }

  return NO_COLLISION;
}

void ObjectSupervisor::Update()
{
  for (auto obj : ground_objects)
    obj->Update();
}

void ObjectSupervisor::DrawObjects()
{
  for (auto obj : all_objects)
    obj->Draw();
}



}

}
