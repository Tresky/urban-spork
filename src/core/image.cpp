#include "../utils/globals.hpp"
#include "image.hpp"

#include "video_manager.hpp"

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



}
