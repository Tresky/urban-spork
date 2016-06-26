#include "map_utils.hpp"
#include <cstdlib>

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

bool MapRectangle::InRange(const MapRectangle& _one, const MapRectangle& _two, const int _range)
{
  if ((_one.right + _range >= _two.left || _one.left - _range <= _two.right) &&
      (_one.bottom + _range >= _two.top || _one.top - _range <= _two.bottom))
  {
    return true;
  }
  return false;
}

} // Private Map Mode namespace

} // Map Mode namespace
