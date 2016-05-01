#include "../../utils/globals.hpp"
#include "global_actors.hpp"

#include "../image.hpp"

namespace rpg_global
{

void GlobalCharacter::SetMapImage(const unsigned int _id)
{
  resource_id = _id;
  GetMapImage()->SetPosition(position.x, position.y);
}

rpg_video::Image* GlobalCharacter::GetMapImage() const
{
  if (resource_id < 0)
    return nullptr;
  return rpg_resource::ResourceManager->GetImage(resource_id);
}

sf::Vector2f GlobalCharacter::GetPosition()
{
  return position;
}

void GlobalCharacter::SetPosition(const int _x, const int _y)
{
  position.x = _x;
  position.y = _y;

  GetMapImage()->SetPosition(position.x, position.y);
}

void GlobalCharacter::Move(const int _x, const int _y)
{
  SetPosition(position.x + _x, position.y + _y);
}

void GlobalCharacter::Draw()
{
  rpg_resource::ResourceManager->DrawImage(resource_id);
}

}
