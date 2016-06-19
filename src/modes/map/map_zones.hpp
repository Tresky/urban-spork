#ifndef MAP_ZONES_HPP
#define MAP_ZONES_HPP

#include "map_utils.hpp"

#include <vector>

namespace rpg_map_mode
{

namespace private_map_mode
{
// class MapRectangle;

class MapZone
{
public:
  MapZone(const int _left, const int _top,
          const int _right, const int _bottom);

  ~MapZone();

  static MapZone* Create(const int _left, const int _top,
                         const int _right, const int _bottom);

  virtual void Update();

  void AddSection(const int _left, const int _top,
                  const int _right, const int _bottom);

  bool IsInside(const MapRectangle& _rect) const;

  virtual void Draw();

protected:
  std::vector<MapRectangle> sections;

private:
  MapZone(const MapZone& map_zone) = delete;
  MapZone& operator=(const MapZone& map_zone) = delete;
};

class CameraZone : public MapZone
{
public:
  CameraZone(const int _left, const int _top,
             const int _right, const int _bottom);

  ~CameraZone();

  static CameraZone* Create(const int _left, const int _top,
                            const int _right, const int _bottom);

  virtual void Update();

  bool IsCameraInside()
  {
    return camera_inside;
  }

protected:
  bool camera_inside;

private:
  CameraZone(const CameraZone& camera_zone) = delete;
  CameraZone& operator=(const CameraZone& camera_zone) = delete;
};

}

}

#endif // MAP_ZONES_HPP
