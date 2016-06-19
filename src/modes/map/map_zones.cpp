#include "../../utils/globals.hpp"
#include "map_zones.hpp"

#include "map_mode.hpp"
#include "map_sprites.hpp"

#include "../../core/video_manager.hpp"

namespace rpg_map_mode
{

namespace private_map_mode
{

MapZone::MapZone(const int _left, const int _top,
                 const int _right, const int _bottom)
{
  AddSection(_left, _top, _right, _bottom);

  MapMode::CurrentInstance()->GetObjectSupervisor()->AddZone(this);
}

MapZone::~MapZone()
{
  // for (MapRectangle rect : sections)
  //   delete rect;
}

MapZone* MapZone::Create(const int _left, const int _top,
                         const int _right, const int _bottom)
{
  return new MapZone(_left, _top, _right, _bottom);
}

void MapZone::Update()
{

}

void MapZone::AddSection(const int _left, const int _top,
                         const int _right, const int _bottom)
{
  sections.push_back(MapRectangle(_left, _top, _right, _bottom));
}

bool MapZone::IsInside(const MapRectangle& _rect) const
{
  for (MapRectangle rect : sections)
    if (MapRectangle::CheckIntersection(rect, _rect))
      return true;
  return false;
}


// void MapZone::Draw()
// {
//     // Verify each section of the zone and check if the position is within the section bounds.
//     for(std::vector<ZoneSection>::const_iterator it = _sections.begin(); it != _sections.end(); ++it) {
//         if(_ShouldDraw(*it)) {
//             vt_video::VideoManager->DrawRectangle((it->right_col - it->left_col) * GRID_LENGTH,
//                                                   (it->bottom_row - it->top_row) * GRID_LENGTH,
//                                                    vt_video::Color(1.0f, 0.6f, 0.0f, 0.6f));
//         }
//     }
// }
//
//
//




void MapZone::Draw()
{
  for (auto map_rect : sections)
  {
    sf::Vector2f pos;
    pos.x = MapMode::CurrentInstance()->GetScreenXCoordinate(map_rect.left);
    pos.y = MapMode::CurrentInstance()->GetScreenYCoordinate(map_rect.top);

    sf::RectangleShape rect(sf::Vector2f(map_rect.right - map_rect.left,
                                         map_rect.bottom - map_rect.top));
    rect.setOutlineColor(sf::Color::Blue);
    rect.setOutlineThickness(1.f);
    rect.setPosition(pos.x, pos.y);
    rect.setFillColor(sf::Color::Transparent);

    rpg_video::VideoManager->DrawShape(rect);
  }
}







CameraZone::CameraZone(const int _left, const int _top,
                       const int _right, const int _bottom)
  : MapZone(_left, _top, _right, _bottom)
  , camera_inside(true)
{}

CameraZone* CameraZone::Create(const int _left, const int _top,
                               const int _right, const int _bottom)
{
  return new CameraZone(_left, _top, _right, _bottom);
}

void CameraZone::Update()
{
  MapZone::Update();

  VirtualSprite* camera = MapMode::CurrentInstance()->GetCamera();
  if (!camera)
    camera_inside = false;
  else if (IsInside(camera->GetGridCollisionRectangle()))
    camera_inside = true;
  else
    camera_inside = false;
}

}

}
