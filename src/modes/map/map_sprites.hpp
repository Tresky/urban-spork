#ifndef MAP_SPRITES_HPP
#define MAP_SPRITES_HPP

#include "map_objects.hpp"

namespace rpg_map_mode
{

namespace private_map_mode
{

class VirtualSprite: public MapObject
{
public:
  VirtualSprite(MapObjectDrawLayer _layer) : MapObject(_layer) {}

  ~VirtualSprite() {}

  virtual void Update() {}

  virtual void Draw() {}

protected:

};

class MapSprite: public VirtualSprite
{
public:
  MapSprite(MapObjectDrawLayer _layer) : VirtualSprite(_layer) {}
private:
};

} // Private Map Mode namespace

} // Map Mode namespace

#endif // MAP_SPRITES_HPP
