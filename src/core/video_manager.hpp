/**
 * \file video_manager.hpp
 *
 * Contains a singleton class for managing
 * all video rendering and window resources.
 * This class is made global across the entire
 * program.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 06-01-16
 */

#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "../utils/singleton.hpp"

namespace rpg_video
{

class VideoEngine;
extern VideoEngine* VideoManager;
extern bool VIDEO_DEBUG;

/**
 * \class VideoEngine
 * \brief Global singleton class that manages the SFML
 * window and handles drawing all resources to the screen.
 *
 * \ingroup VideoEngine
 *
 * This class is rather large, but it handles all functionality
 * that is associated with the SFML window. This class seeks to
 * distance the programmer from working directly in SFML as much
 * as possible. All drawing, clearing, displaying, etc in SFML is
 * wrapped inside of this class and it can be used globally across
 * the entire program.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 06-01-16
 */
class VideoEngine : public rpg_utils::Singleton<VideoEngine>
{
  friend class rpg_utils::Singleton<VideoEngine>;
public:
  /**
   * Destructor
   */
  ~VideoEngine()
  {
    IF_PRINT_DEBUG(VIDEO_DEBUG) << "VideoEngine destructor called" << endl;

    if (window->isOpen())
      window->close();
    if (window)
      delete window;
  }

  /**
   * Inherited from Singleton; initialize the
   * VideoManager singleton object.
   * \return True if successful, false otherwise
   */
  bool InitSingleton()
  {
    return true;
  }

  /**
   * Generate an SFML window handle based on parameters.
   * \param _width  Width of window
   * \param _height Height of window
   * \param _title  Title to place in window
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
   * \return Pointer to the SFML window handle
   */
  sf::RenderWindow* GetWindowHandle() const
  {
    return window;
  }

  /**
   * Clear the entire window with black.
   */
  void Clear() const
  {
    Clear(sf::Color::Black);
  }

  /**
   * Clear the entire window with an arbitrary color.
   * \param _color sf::Color to clear the screen to
   */
  void Clear(const sf::Color _color) const
  {
    window->clear(_color);
  }

  /**
   * Mainly only updates the FPS calculations.
   */
  void Update();

  /**
   * Draws all debug information to the screen.
   */
  void DrawDebugInfo() const;

  /**
   * Returns the current width of the screen window.
   * \return Unsigned integer of the screen width
   */
  unsigned int GetScreenWidth() const
  {
    return screen_width;
  }

  /**
   * Returns the current height of the screen window.
   * \return Unsigned integer of the screen height
   */
  unsigned int GetScreenHeight() const
  {
    return screen_height;
  }

  /**
   * Draws an SFML sprite to the window.
   * \param _sprite SFML sprite to draw
   */
  void DrawSprite(sf::Sprite& _sprite) const;

  /**
   * Draws an SFML shape to the window.
   * @param _shape SFML shape to draw
   */
  void DrawShape(sf::Shape& _shape) const;

  /**
   * Display all objects that have been drawn
   * onto the window handle.
   *
   * \note Nothing will appear on the screen
   * until this function is called; call it once
   * every frame after drawing.
   */
  void Display() const;

  /**
   * Closes the SFML window handle.
   */
  void Close() const;

  /**
   * Draws a line to the SFML window handle. This
   * function requires two coordinates and draws
   * a line between them.
   * \param _x1    X position of starting point
   * \param _y1    Y position of starting point
   * \param _x2    X position of ending point
   * \param _y2    Y position of ending point
   * \param _width Number of pixels wide to draw
   * \param _color SFML color to draw line with
   */
  void DrawLine(const int _x1, const int _y1,
                const int _x2, const int _y2,
                const int _width, const sf::Color& _color) const;

  /**
   * Draws a grid layout onto the SFML window handle.
   * This function requires two coordinates representing
   * the top-left and bottom-right positions of the grid.
   * \param left        Left position of grid
   * \param top         Top position of grid
   * \param right       Right position of grid
   * \param bottom      Bottom position of grid
   * \param width_cell  Width of grid cell
   * \param height_cell Height of grid cell
   * \param width_line  Width of grid lines to draw
   * \param color       SFML color to draw grid with
   *
   * \note This function uses the DrawLine() function to draw
   * its grid line. \see DrawLine()
   */
  void DrawGrid(const float left, const float top,
                const float right, const float bottom,
                const float width_cell,
                const float height_cell,
                const unsigned width_line, const sf::Color& color) const;

private:
  /**
   * Default private constructor
   */
  VideoEngine();

  /**
   * Private function to calculate the FPS
   * every game cycle.
   */
  void UpdateFPS();

  /**
   * Draw the FPS to the SFML window handle.
   */
  void DrawFPS() const;

  //! SFML window handle
  sf::RenderWindow* window;

  //! Text object to display the fps in debug mode
  sf::Text fps_text;
  //! Font object to associate to the FPS text
  sf::Font font;

  //! Width of the screen
  unsigned int screen_width;
  //! Height of the screen
  unsigned int screen_height;
}; // class VideoEngine

} // namespace rpg_video

#endif // VIDEO_HPP
