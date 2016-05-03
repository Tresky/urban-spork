#include "../../utils/globals.hpp"
#include "global_actors.hpp"

#include "../script_read.hpp"
#include "../image.hpp"

using namespace rpg_script;

namespace rpg_global
{

bool GlobalCharacter::LoadAnimations(const std::string& _filepath)
{
  // cout << "hola" << endl;
  ReadScript sprite_script;
  cout << "Opening Script: " << _filepath << endl;
  if (!sprite_script.OpenFile(_filepath))
  {
    PRINT_ERROR << "Failed to open spritesheet script: " << _filepath << endl;
    return false;
  }
cout << "hello" << endl;
  if (!sprite_script.OpenTable("animations"))
  {
    PRINT_ERROR << "Table doesn't exist in script: animations" << endl;
    return false;
  }

  string filepath = sprite_script.ReadData<std::string>("filepath", "");
  cout << "FilePath : " << filepath << endl;

  return true;
}

void GlobalCharacter::SetCurrentAnimation(const std::string& _key)
{
  if (!_key.empty())
    current_animation = _key;
}

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

  //GetMapImage()->SetPosition(position.x, position.y);
}

void GlobalCharacter::Move(const int _x, const int _y)
{
  SetPosition(position.x + _x, position.y + _y);
}

void GlobalCharacter::Update()
{
  // if (animations.find(current_animation) != animations.end())
  //   animations[current_animation]->Update();
}

void GlobalCharacter::Draw()
{
  // if (animations.find(current_animation) != animations.end())
  //   animations[current_animation]->Draw();
  // rpg_resource::ResourceManager->DrawImage(resource_id);
}

}
