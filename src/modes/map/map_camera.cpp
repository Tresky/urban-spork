#include "../../utils/globals.hpp"
#include "map_camera.hpp"

#include "../../core/video_manager.hpp"

namespace rpg_map_mode
{

namespace private_map_mode
{

Camera::Camera(const sf::Vector2i _tile_dimensions)
  : need_update(false)
  , position(0.f, 0.f)
  , target_position(position)
  , tile_dimensions(_tile_dimensions)
{
  InitializeCamera(_tile_dimensions);
}

bool Camera::InitializeCamera(const sf::Vector2i _tile_dimensions)
{
  tile_dimensions = _tile_dimensions;
}

sf::Vector2f Camera::GetCenterPosition()
{
  return sf::Vector2f(position.x + rpg_video::VideoManager->GetScreenWidth() / 2.f,
                      position.y + rpg_video::VideoManager->GetScreenHeight() / 2.f);
}

void Camera::SetPosition(const int _x, const int _y)
{
  position.x = _x;
  position.y = _y;
}

void Camera::SetCenterPosition(const int _x, const int _y)
{
  SetPosition(_x - rpg_video::VideoManager->GetScreenWidth() / 2.f,
              _y - rpg_video::VideoManager->GetScreenHeight() / 2.f);
}

void Camera::MoveTo(const int _x, const int _y)
{
  target_position.x = _x;
  target_position.y = _y;
  need_update = true;
}

void Camera::MoveCenterTo(const int _x, const int _y)
{
  MoveTo(_x - rpg_video::VideoManager->GetScreenWidth() / 2.f,
         _y - rpg_video::VideoManager->GetScreenHeight() / 2.f);
}

void Camera::Update()
{
  if (need_update)
    CalculatePosition();
}

sf::Vector2i Camera::GetTileOffset()
{
  sf::Vector2i temp;
  temp.x = static_cast<int>(position.x) % tile_dimensions.x;
  temp.y = static_cast<int>(position.y) % tile_dimensions.y;
  return temp;
}

sf::IntRect Camera::GetCameraBounds()
{
  if (tile_dimensions.x == 0 || tile_dimensions.y == 0)
    return sf::IntRect(0, 0, 0, 0);

  int tile_x = static_cast<int>(position.x / tile_dimensions.x);
  int tile_y = static_cast<int>(position.y / tile_dimensions.y);

  /// If the whole size of the camera is not divisible by the tile
  /// size, we need to add one more tile to the bounds.
  /// We also have to add one to the numbers to make the tile count
  /// begin at 1 as opposed to 0.
  int tiles_across = static_cast<int>(rpg_video::VideoManager->GetScreenWidth() / tile_dimensions.x) + 2;
  int tiles_down = static_cast<int>(rpg_video::VideoManager->GetScreenHeight() / tile_dimensions.y) + 2;

  /// If we are not perfectly positioned over the tiles, we need to
  /// add one more tile to the frame to make sure no black is shown.
  if (static_cast<int>(position.x) % tile_dimensions.x != 0)
    ++tiles_across;
  if (static_cast<int>(position.y) % tile_dimensions.y != 0)
    ++tiles_down;

  return sf::IntRect(tile_x, tile_y, tiles_across, tiles_down);
}

void Camera::CalculatePosition()
{
  const float x_dist = static_cast<float>(target_position.x - position.x);
  const float y_dist = static_cast<float>(target_position.y - position.y);

  const float distance = sqrt(pow(x_dist, 2) + pow(y_dist, 2));

  /// A spin-off of Xeno's Paradox prohibits us from reaching the
  /// target, so we can counter that by giving up when we are within 1 pixel.
  if (distance <= 1.f)
  {
    position = target_position;
    need_update = false;
  }
  else
  {
    float velocity = distance / 60.f;

    /// If our velocity falls below 1.0, we will never enter
    /// Sovngarde!
    if (velocity < 1.f)
      velocity = 1.f;

    const float velocity_x = x_dist * (velocity / distance);
    const float velocity_y = y_dist * (velocity / distance);

    position.x += velocity_x;
    position.y += velocity_y;
  }
}

}

}
