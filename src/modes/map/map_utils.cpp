#include "map_utils.hpp"

#include <iostream>
namespace rpg_map_mode
{

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


}


}
