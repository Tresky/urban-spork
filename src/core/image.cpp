#include "../utils/globals.hpp"
#include "image.hpp"

#include "video_manager.hpp"
#include "system.hpp"

namespace rpg_video
{

Image::Image(const Image& _copy)
  : resource(_copy.resource)
  , position(0.f, 0.f)
{}

Image& Image::operator=(const Image& _copy)
{
  if (this != &_copy)
  {
    Image copy(_copy);

    std::swap(resource, copy.resource);
    std::swap(position, copy.position);
  }

  return *this;
}

Image::~Image()
{
  delete resource;
  resource = nullptr;
}

bool Image::LoadResource(const std::string& _filepath)
{
  if (_filepath.empty())
  {
    PRINT_ERROR << "Attempted to load image from invalid filepath" << endl;
    return false;
  }

  if (!resource->loadFromFile(_filepath))
  {
    PRINT_ERROR << "Failed to load file: " << _filepath << endl;
    return false;
  }

  sprite.setTexture(*resource);
  SetPosition(position.x, position.y);
  return true;
}

bool Image::LoadResourceFromImage(const sf::Image& _image, sf::IntRect _rect)
{
  if (!resource->loadFromImage(_image, _rect))
  {
    PRINT_ERROR << "Failed to load image resource" << endl;
    return false;
  }

  sprite.setTexture(*resource);
  SetPosition(position.x, position.y);
  return true;
}

unsigned int Image::GetWidth() const
{
  return sprite.getTextureRect().width;
}

unsigned int Image::GetHeight() const
{
  return sprite.getTextureRect().height;
}

void Image::SetPosition(const unsigned int _x, const unsigned int _y)
{
  position.x = _x;
  position.y = _y;
  sprite.setPosition(position);
}

void Image::Draw()
{
  VideoManager->DrawSprite(sprite);
}

AnimatedImage::AnimatedImage(const unsigned int _width,
                             const unsigned int _height)
  : current_frame(0)
  , frame_counter(0)
  , number_loops(-1)
  , loop_counter(0)
  , finished(false)
{}

bool AnimatedImage::LoadSpritesheetScript(const std::string &_filepath)
{

  // ReadScript sprite_script;
  // if (!sprite_script.OpenFile(_filepath))
  // {
  //   PRINT_ERROR << "Failed to open spritesheet script: " << _filepath << endl;
  //   return false;
  // }
  //
  // if (!sprite_script.OpenTable("animations"))
  // {
  //   PRINT_ERROR << "Table doesn't exist in script: animations" << endl;
  //   return false;
  // }
  //
  // string filepath = sprite_script.ReadData<std::string>("filepath", "");
  // cout << "FilePath : " << filepath << endl;
  //
  // return true;
}

void AnimatedImage::Draw()
{
  if (frames.empty())
  {
    IF_PRINT_DEBUG(VIDEO_DEBUG) << "No frames in animation" << endl;
    return;
  }

  frames[current_frame].image.Draw();
}

void AnimatedImage::Reset()
{
  current_frame = 0;
  frame_counter = 0;
  loop_counter = 0;
  finished = false;
}

void AnimatedImage::Update()
{
  // If we have 1 or less frames, no need to update
  // Also if we are "finished", we don't need to update
  if (frames.size() <= 1 || finished)
    return;

  // Get the elapsed time from the SystemManager
  unsigned int elapsed_time = rpg_system::SystemManager->GetUpdateTime();

  while (frame_counter >= frames[current_frame].frame_time)
  {
    // If frame_time is equal to zero, it's a terminating frame.
    if (frames[current_frame].frame_time == 0)
    {
      finished = true;
      return;
    }

    elapsed_time = frame_counter - frames[current_frame++].frame_time;
    if (current_frame >= frames.size())
    {
      if (number_loops >= 0 && ++loop_counter >= number_loops)
      {
        finished = true;
        frame_counter = 0;
        current_frame = frames.size() - 1;
        return;
      }
      current_frame = 0;
    }
    frame_counter = elapsed_time;
  }
}

}
