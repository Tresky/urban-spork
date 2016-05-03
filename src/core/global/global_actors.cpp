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
    throw Exception("Failed to open spritesheet script: " + _filepath);

  if (!sprite_script.OpenTable("animations"))
    throw Exception("Table doesn't exist in script: animations");

  string filepath = sprite_script.ReadData<std::string>("filepath", "");
  if (filepath.empty())
    return false;

  if (!sprite_script.OpenTableIntegers(0))
    throw Exception("Table doesn't exist in script: 0");
    cout << "ajshdkajshd" << endl;

  string animation_name = sprite_script.ReadData<std::string>("name", "");
  if (animation_name.empty())
    return false;

  int num_frames = sprite_script.ReadData<int>("num_frames", -1);
  if (num_frames == -1)
    return false;


  if (!sprite_script.OpenTable("frame_rects"))
    throw Exception("Table doesn't exist in script: frame_rects");

  sprite_script.BookmarkCurrentTable();

  animations[animation_name] = new AnimatedImage(32, 32);
  for (int i = 1; i < num_frames + 1; ++i)
  {
    sprite_script.OpenTableIntegers(i);
    int x = sprite_script.ReadData<int>("x", -1);
    int y = sprite_script.ReadData<int>("y", -1);
    if (x < 0 || y < 0)
      return false;

    sf::IntRect rect = sf::IntRect(x, y, 32, 32);
    int resource_id = ResourceManager->LoadImageRect(filepath, rect);
    if (resource_id < 0)
      return false;

    animations[animation_name]->AddFrame(resource_id);
    sprite_script.LoadBookmark();
  }

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

  animations[current_animation]->SetPosition(_x, _y);

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
