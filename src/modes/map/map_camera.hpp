#ifndef MAP_CAMERA_HPP
#define MAP_CAMERA_HPP

#include <SFML/Graphics.hpp>

namespace rpg_map_mode
{

namespace private_map_mode
{

class Camera
{
public:
  Camera(const sf::Vector2i _tile_dimensions);

  bool InitializeCamera(const sf::Vector2i _tile_dimensions);

  void SetPosition(const int _x, const int _y);
  void SetCenterPosition(const int _x, const int _y);

  void MoveTo(const int _x, const int _y);
  void MoveCenterTo(const int _x, const int _y);

  sf::Vector2f GetPosition()
  {
    return position;
  }

  sf::Vector2f GetCenterPosition();

  void Update();

  sf::Vector2i GetTileOffset();
  sf::IntRect GetCameraBounds();


private:
  void CalculatePosition();

  bool need_update;

  sf::Vector2f position;

  sf::Vector2f target_position;

  sf::Vector2f screen_dimensions;

  sf::Vector2i tile_dimensions;

  sf::IntRect camera_bounds;
};

}

}

#endif // MAP_CAMERA_HPP
