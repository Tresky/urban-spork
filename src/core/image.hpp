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

namespace private_video
{

class AnimationFrame
{
public:
  // Time to show frame
  unsigned int frame_time;

  // Image which to show
  Image image;
};

}

class AnimatedImage
{
public:
  /**
   * Constructor
   * @param  _width  Width of animation frame
   * @param  _height Height of animation frame
   */
  explicit AnimatedImage(const unsigned int _width,
                         const unsigned int _height);

  /**
   * Loads the animation data from a Lua script denoted
   * by the parameter name.
   * @param  _filepath String of file location to Lua script
   * @return           True if successful, false otherwise
   */
  bool LoadSpritesheetScript(const std::string &_filepath);

  /**
   * Draws the current frame of the animation.
   */
  void Draw();

  /**
   * Resets the animation to its inital state.
   */
  void Reset();

  /**
   * Updates the animation to the next frame after the
   * timer is up.
   */
  void Update();


  /**
   * Returns the number of frames in the animation.
   * @return Integer for the number of frames
   */
  unsigned int GetNumberFrames()
  {
    return frames.size();
  }

  /**
   * Returns an Image pointer to the current frame.
   * @return Image* for the current frame
   */
  Image* GetCurrentFrame()
  {
    return &frames[current_frame].image;
  }

  /**
   * Returns the index to the currnent frame.
   * @return Integer for the current frame index
   */
  unsigned int GetCurrentFrameIndex()
  {
    return current_frame;
  }

  /**
   * Sets the number of loops to complete. A number
   * less than 0 will indicate looping indefinitely.
   * @param _num_loops Integer for number of loops
   */
  void SetNumberLoops(const int _num_loops)
  {
    number_loops = _num_loops;
    if (loop_counter >= number_loops && number_loops >= 0)
      finished = true;
  }

  /**
   * Set the current number of completed loops.
   * @param _loops Integer for the current completed loops
   */
  void SetLoopCounter(const unsigned int _loops)
  {
    loop_counter = _loops;
    if (loop_counter >= number_loops && number_loops >= 0)
      finished = true;
  }

  /**
   * Set the animation as finished as not.
   * @param _finished True will stop the animation, false starts it
   */
  void SetFinished(const bool _finished)
  {
    finished = _finished;
    if (finished == false)
      loop_counter = 0;
  }

private:
  // Index of the current frame in the animation
  unsigned int current_frame;

  // Time each frame has been shown; for timing purposes
  unsigned int frame_counter;

  // Number of loops to complete; -1 means infinite
  signed int number_loops;

  // Total number of loops completed
  unsigned int loop_counter;

  // Set to true to stop the animation; false to play
  bool finished;

  // Vector containing the images for the frames
  std::vector<private_video::AnimationFrame> frames;


};

}

#endif // IMAGE_HPP
