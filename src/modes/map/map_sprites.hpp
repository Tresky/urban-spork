#ifndef MAP_SPRITES_HPP
#define MAP_SPRITES_HPP

#include "map_objects.hpp"

namespace rpg_video
{ class AnimatedImage; }

namespace rpg_map_mode
{

enum Direction
{
  DIRECTION_INVALID = 0,
  DIRECTION_NORTH = 1,
  DIRECTION_NORTHWEST = 2,
  DIRECTION_NORTHEAST = 3,
  DIRECTION_SOUTH = 4,
  DIRECTION_SOUTHWEST = 5,
  DIRECTION_SOUTHEAST = 6,
  DIRECTION_EAST = 7,
  DIRECTION_WEST = 8
};

namespace private_map_mode
{

class VirtualSprite: public MapObject
{
public:
  VirtualSprite(MapObjectDrawLayer _layer);

  ~VirtualSprite() {}

  virtual void Update();

  virtual void Draw() {}

  void SetDirection(const Direction _direction);

  const Direction GetDirection()
  {
    return direction;
  }

  float CalculateDistanceMoved();

  void SetMoving(bool _moving)
  {
    moving = _moving;
  }

  bool IsMoving()
  {
    return moving;
  }

  bool HasMoved()
  {
    return has_moved;
  }

protected:
  void SetNextPosition();

  float movement_speed;

  bool moving;

  bool has_moved;

  Direction direction;
};

class MapSprite: public VirtualSprite
{
public:
  MapSprite(MapObjectDrawLayer _layer);

  ~MapSprite();

  bool LoadAnimations(const std::string& _filepath);

  void SetCurrentAnimation(const std::string& _current_animation)
  {
    current_animation = _current_animation;
  }

  virtual void Update();

  virtual void Draw();

protected:

  std::string current_animation;

  std::map<std::string, rpg_video::AnimatedImage*> animations;
};

class EnemySprite: public MapSprite
{
public:
  EnemySprite();

  virtual ~EnemySprite();

  static EnemySprite* Create();

  virtual void Update();

  virtual void Draw();

  bool IsDead() const
  {
    return state == State::DEAD;
  }

  bool IsSpawning() const
  {
    return state == State::SPAWNING;
  }

  bool IsHostile() const
  {
    return state == State::HOSTILE;
  }

private:
  enum State
  {
    SPAWNING = 0,
    HOSTILE,
    DEAD
  } state;


};

} // Private Map Mode namespace

} // Map Mode namespace

#endif // MAP_SPRITES_HPP
