#include "../utils/globals.hpp"
#include "resources.hpp"

#include "image.hpp"

namespace rpg_resource
{

ResourceEngine* ResourceManager = nullptr;
bool RESOURCE_DEBUG = true;

ResourceEngine::ResourceEngine()
  : counter(0)
{
  IF_PRINT_DEBUG(RESOURCE_DEBUG) << "ResourceEngine constructor called" << endl;
}

ResourceEngine::~ResourceEngine()
{
  IF_PRINT_DEBUG(RESOURCE_DEBUG) << "ResourceEngine destructor called" << endl;
  // TODO: Iterate through images and delete them
}

bool ResourceEngine::InitSingleton()
{
  return true;
}

unsigned int ResourceEngine::LoadImage(const std::string& _filepath)
{
  int temp_id = 0;
  if (reusable_ids.empty())
  {
    temp_id = counter++;
    images[temp_id] = new rpg_video::Image();
  }
  else
  {
    temp_id = reusable_ids.front();
    images[temp_id] = new rpg_video::Image();
    reusable_ids.pop();
  }

  if (!images[temp_id]->LoadResource(_filepath))
  {
    PRINT_ERROR << "Failed to load image: " << _filepath << endl;

    // Clean up
    delete images[temp_id];
    images[temp_id] = nullptr;

    // Set temp_id as reusable
    reusable_ids.push(temp_id);
    return -1;
  }

  return temp_id;
}

unsigned int ResourceEngine::LoadImageRect(const std::string& _filepath, sf::IntRect _rect)
{
  int temp_id = 0;
  if (reusable_ids.empty())
  {
    temp_id = counter++;
    images[temp_id] = new rpg_video::Image();
  }
  else
  {
    temp_id = reusable_ids.front();
    images[temp_id] = new rpg_video::Image();
    reusable_ids.pop();
  }

  // We have a cache miss.
  // This is important because we don't want to have to
  // reload the tilesheet from disk for every tile we
  // load into the ResourceEngine.
  if (image_cache.first != _filepath)
  {
    if (!image_cache.second.loadFromFile(_filepath))
    {
      // Cache is now corrupted, so it needs to
      // be cleared.
      image_cache.first = "";

      // Clean up bad resources
      delete images[temp_id];
      images[temp_id] = nullptr;

      // Set temp_id as reusable
      reusable_ids.push(temp_id);
      return -1;
    }
    else
      // Set the cache as valid
      image_cache.first = _filepath;
  }

  // Load the resource from the tileset image
  if (!images[temp_id]->LoadResourceFromImage(image_cache.second, _rect))
  {
    PRINT_ERROR << "Failed to load image" << endl;

    // Clean up
    delete images[temp_id];
    images[temp_id] = nullptr;

    // Set temp_id as reusable
    reusable_ids.push(temp_id);
    return -1;
  }

  return temp_id;
}

rpg_video::Image* ResourceEngine::GetImage(const unsigned int _id)
{
  if (_id >= images.size())
  {
    IF_PRINT_DEBUG(RESOURCE_DEBUG) << "Requested image id invalid" << endl;
    return nullptr;
  }

  return images[_id];
}

void ResourceEngine::DrawImage(const unsigned int _id)
{
  GetImage(_id)->Draw();
}

}
