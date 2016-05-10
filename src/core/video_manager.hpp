#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "../utils/singleton.hpp"

namespace rpg_video
{

class VideoEngine;
extern VideoEngine* VideoManager;
extern bool VIDEO_DEBUG;

class VideoEngine : public rpg_utils::Singleton<VideoEngine>
{
  friend class rpg_utils::Singleton<VideoEngine>;
public:
  ~VideoEngine()
  {
    IF_PRINT_DEBUG(VIDEO_DEBUG) << "VideoEngine destructor called" << endl;

    if (window->isOpen())
      window->close();
    delete window;
  }

  bool InitSingleton()
  {
    return true;
  }

  /**
   * Generate an SFML window handle based on parameters.
   * @param _width  Width of window
   * @param _height Height of window
   * @param _title  Title to place in window
   */
  void CreateWindow(unsigned int _width, unsigned int _height, string _title)
  {
    if (!window)
    {
      screen_width = _width;
      screen_height = _height;
      window = new sf::RenderWindow(sf::VideoMode(screen_width, screen_height), _title);
      window->setFramerateLimit(60);
      IF_PRINT_DEBUG(VIDEO_DEBUG) << "Window created" << endl;
    }
    else
      IF_PRINT_DEBUG(VIDEO_DEBUG) << "Window already created" << endl;
  }

  /**
   * Returns the SFML window handle.
   * @return Pointer to the SFML window handle
   */
  sf::RenderWindow* GetWindowHandle()
  {
    return window;
  }

  /**
   * Clear the entire window with gray.
   */
  void Clear()
  {
    Clear(sf::Color::Black);
  }

  /**
   * Clear the entire window with an arbitrary color.
   * @param _color sf::Color to clear the screen to
   */
  void Clear(const sf::Color _color)
  {
    window->clear(_color);
  }

  void Update();

  void DrawDebugInfo();

  unsigned int GetScreenWidth() const
  {
    return screen_width;
  }

  unsigned int GetScreenHeight() const
  {
    return screen_height;
  }

  void DrawSprite(sf::Sprite& _sprite);

  void DrawShape(sf::Shape& _shape);

  void Display();

  void Close();

  void DrawLine(const int _x1, const int _y1,
                const int _x2, const int _y2,
                const int _width, const sf::Color& _color);

  void DrawGrid(const float left, const float top,
                const float right, const float bottom,
                const float width_cell_horizontal,
                const float width_cell_vertical,
                const unsigned width_line, const sf::Color& color);

private:
  VideoEngine();

  void UpdateFPS();

  void DrawFPS();

  // SFML window handle
  sf::RenderWindow* window;

  // Text object to display the fps in debug mode
  sf::Text fps_text;
  sf::Font font;

  // Width and height of the screen handle
  unsigned int screen_width;
  unsigned int screen_height;
};

}

#endif // VIDEO_HPP
