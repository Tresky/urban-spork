#include "../../utils/globals.hpp"
#include "global_actors.hpp"

#include "../script_read.hpp"
#include "../image.hpp"

using namespace rpg_script;
using namespace rpg_resource;
using namespace rpg_video;

namespace rpg_global
{

bool GlobalCharacter::LoadAnimations(const std::string& _filepath)
{
  ReadScript sprite_script;

  if (!sprite_script.OpenFile(_filepath))
  {
    PRINT_WARNING << "Failed to open animations script: " << _filepath << endl;
    return false;
  }

  sprite_script.OpenTable("animations"); // Opens animations

  string filepath = sprite_script.ReadData<std::string>("filepath", "");
  int num_animations = sprite_script.ReadData<int>("num_animations", -1);

  // Check for any errors so far
  if (sprite_script.HasError())
  {
    sprite_script.PrintErrors();
    return false;
  }

  // Loop through all animation tables
  for (int i = 0; i < num_animations; ++i)
  {
    sprite_script.OpenTableIntegers(i); // Opens animation[i]

    string animation_name = sprite_script.ReadData<std::string>("name", "");

    int tile_width = sprite_script.ReadData<int>("width", -1);
    int tile_height = sprite_script.ReadData<int>("width", -1);

    int num_frames = sprite_script.ReadData<int>("num_frames", -1);
    int frame_time = sprite_script.ReadData<int>("frame_time", -1);

    sprite_script.OpenTable("frame_rects"); // Opens frame_rects

    // Check for any errors so far
    if (sprite_script.HasError())
    {
      sprite_script.PrintErrors();
      return false;
    }

    animations[animation_name] = new AnimatedImage(tile_width, tile_height);
    for (int j = 1; j < num_frames + 1; ++j)
    {
      sprite_script.OpenTableIntegers(j); // Open {x, y}

      int x = sprite_script.ReadData<int>("x", -1);
      int y = sprite_script.ReadData<int>("y", -1);

      // Check for any errors so far
      if (sprite_script.HasError())
      {
        sprite_script.PrintErrors();
        delete animations[animation_name];
        animations[animation_name] = nullptr;
        return false;
      }

      sf::IntRect rect = sf::IntRect(x, y, tile_width, tile_height);
      int resource_id = ResourceManager->LoadImageRect(filepath, rect);
      if (resource_id < 0)
      {
        // If the ResourceManager failed to load the resource, we need
        // to clean up a hanging pointer.
        PRINT_WARNING << "Failed to load resource from: " << filepath << endl;
        delete animations[animation_name];
        animations[animation_name] = nullptr;
        return false;
      }

      animations[animation_name]->AddFrame(resource_id, frame_time);
      sprite_script.CloseTable(); // Closes {x, y}
    }

    sprite_script.CloseTable(); // Closes frame_rects
    sprite_script.CloseTable(); // Closes animation[i]
  }

  return true;
}

void GlobalCharacter::SetCurrentAnimation(const std::string& _key)
{
  if (!_key.empty())
    current_animation = _key;
}

void GlobalCharacter::SetDirection(const std::string& _direction)
{
  if (!_direction.empty())
    direction = _direction;
}

std::string GlobalCharacter::GetDirection()
{
  return direction;
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

  for (auto anim : animations)
    anim.second->SetPosition(_x, _y);

  //GetMapImage()->SetPosition(position.x, position.y);
}

void GlobalCharacter::Move(const int _x, const int _y)
{
  SetPosition(position.x + _x, position.y + _y);
}

void GlobalCharacter::Update()
{  
  if (animations.find(current_animation) != animations.end())
    animations[current_animation]->Update();
}

void GlobalCharacter::Draw()
{
  if (animations.find(current_animation) != animations.end())
    animations[current_animation]->Draw();
  //rpg_resource::ResourceManager->DrawImage(resource_id);
}

}
