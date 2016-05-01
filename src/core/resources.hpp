#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <SFML/Graphics.hpp>
#include <queue>

#include "../utils/singleton.hpp"

namespace rpg_video { class Image; }

namespace rpg_resource
{

class ResourceEngine;
extern ResourceEngine* ResourceManager;
extern bool RESOURCE_DEBUG;

/**
 *
 */
class ResourceEngine : public rpg_utils::Singleton<ResourceEngine>
{
  friend class rpg_utils::Singleton<ResourceEngine>;
public:
  ~ResourceEngine();

  bool InitSingleton();

  /**
   * Loads an image into the ResourcEngine and returns
   * a global id to it's pointer.
   * @param  _filepath Filepath to image to load
   * @return           Global id to access image pointer
   */
  unsigned int LoadImage(const std::string& _filepath);

  /**
   * [LoadImageRect description]
   * @param  _filepath [description]
   * @param  _rect     [description]
   * @return           [description]
   */
  unsigned int LoadImageRect(const std::string& _filepath, sf::IntRect _rect);

  //unsigned int StoreImage(sf::Texture)

  /**
   * Returns a pointer to the image specified.
   * @param  _id Global id of the image to access
   * @return     Pointer to image
   */
  rpg_video::Image* GetImage(const unsigned int _id);

  void DrawImage(const unsigned int _id);

private:
  ResourceEngine();

  std::pair<std::string, sf::Image> image_cache;

  std::map<int, rpg_video::Image*> images;

  std::queue<int> reusable_ids;

  unsigned int counter;
};

} // namespace rpg_resource

#endif // RESOURCES_HPP
