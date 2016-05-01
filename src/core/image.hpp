#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <SFML/Graphics.hpp>

namespace rpg_resource
{ class ResourceEngine; }

namespace rpg_video
{
class VideoEngine;

/**
 *
 */
class Image
{
  friend class VideoEngine;
  friend class rpg_resource::ResourceEngine;
public:
  /**
   * Constructor
   */
  Image()
    : resource(new sf::Texture())
  {};

  /**
   * Copy constructor
   */
  Image(const Image& _copy);

  /**
   * Copy assignment operator
   */
  Image& operator=(const Image& _copy);

  /**
   * Destructor
   */
  ~Image();

  /**
   * Loads an image file from a specified path.
   * @param  _filepath Path to load image from
   * @return           True if successful, false otherwise
   */
  bool LoadResource(const string& _filepath);

  /**
   * Loads a texture resource from a section of
   * an SFML image passed in.
   * @param  _image SFML image to load from
   * @param  _rect  SFML rectangle to load
   * @return        True if successful, false otherwise
   */
  bool LoadResourceFromImage(const sf::Image& _image, sf::IntRect _rect);

  /**
   * Returns the width of the image resource.
   * @return Unsigned int of width of image
   */
  unsigned int GetWidth() const;

  /**
   * Returns the height of the image resource.
   * @return Unsigned int of height of image
   */
  unsigned int GetHeight() const;

  /**
   * Sets the position of the sprite associated
   * with the Image object.
   * @param _x X position
   * @param _y Y position
   */
  void SetPosition(const unsigned int _x, const unsigned int _y);

  /**
   * Draw the resource using the VideoEngine
   */
  void Draw();

private:
  // SFML image resource
  sf::Texture* resource;

  // Drawable resource
  sf::Sprite sprite;

  // Position of the image on the screen
  sf::Vector2f position;
};

}

#endif // IMAGE_HPP
