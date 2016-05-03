#include "map_utils.hpp"

// Map Mode namespace
namespace rpg_map_mode
{

// Private Map Mode namespace
namespace private_map_mode
{

bool MapRectangle::CheckIntersection(const MapRectangle& _one, const MapRectangle& _two)
{
  if ((_one.left > _two.right) ||
      (_one.right < _two.left) ||
      (_one.top > _two.bottom) ||
      (_one.bottom < _two.top))
    return false;
  return true;
}

} // Private Map Mode namespace

} // Map Mode namespace
