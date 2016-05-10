#include "../utils/globals.hpp"
#include "video_manager.hpp"

// For std::to_string()
#include <string>

#include "system.hpp"

using namespace rpg_mode_manager;
using namespace rpg_system;

namespace rpg_video
{

VideoEngine* VideoManager = nullptr;
bool VIDEO_DEBUG = true;

VideoEngine::VideoEngine()
  : window(nullptr)
  , screen_width(0)
  , screen_height(0)
{
  IF_PRINT_DEBUG(VIDEO_DEBUG) << "VideoEngine constructor called" << endl;

  if (!font.loadFromFile("data/fonts/default.ttf"))
    PRINT_ERROR << "Font failed to load" << endl;
  else
    fps_text.setFont(font);
  fps_text.setPosition(5, 5);
  fps_text.setColor(sf::Color::White);
  fps_text.setCharacterSize(24);
}

void VideoEngine::Update()
{
  UpdateFPS();
}

void VideoEngine::DrawDebugInfo()
{
  DrawFPS();
}

void VideoEngine::DrawSprite(sf::Sprite& _sprite)
{
  window->draw(_sprite);
}

void VideoEngine::DrawShape(sf::Shape& _shape)
{
  window->draw(_shape);
}

void VideoEngine::Display()
{
  window->display();
}

void VideoEngine::Close()
{
  window->close();
}

void VideoEngine::UpdateFPS()
{
  int frame_time = SystemManager->GetUpdateTime();

  //cout << "------ " << frame_time << endl;
  //cout << "UT: " << SystemManager->GetUpdateTime();
  //int current_fps = num_frames++ / SystemManager->GetUpdateTime();
  int current_fps = 1000 / frame_time;

  fps_text.setString("FPS: " + std::to_string(current_fps));

  //IF_PRINT_DEBUG(VIDEO_DEBUG) << "FPS: " << current_fps << endl;
}

void VideoEngine::DrawFPS()
{
  window->draw(fps_text);
}

void VideoEngine::DrawLine(const int _x1, const int _y1,
                           const int _x2, const int _y2,
                           const int _width, const sf::Color& _color)
{
  sf::VertexArray line(sf::Lines, 2);
  line[0].position = sf::Vector2f(_x1, _y1);
  line[1].position = sf::Vector2f(_x2, _y2);

  window->draw(line);
  // sf::RectangleShape line;
  // line.setPosition(_x1, _y1);
  // line.set
  // line.setLineThickness(_width);
}

void VideoEngine::DrawGrid(const float _left, const float _top,
                           const float _right, const float _bottom,
                           const float _width_cell_horizontal,
                           const float _width_cell_vertical,
                           const unsigned _width_line, const sf::Color& _color)
{
  // #include <cassert>
  // using std::assert;
  // assert(_right > _left);
  // assert(_bottom > _top);
  // assert(_width_cell_horizontal > 0.0f);
  // assert(_width_cell_vertical > 0.0f);
  // assert(_width_line > 0);

  // Draw the grid's vertical lines.
  for (float i = _left; i <= _right; i += _width_cell_horizontal)
      DrawLine(i, _top, i, _bottom, _width_line, _color);

  // Draw the grid's horizontal lines.
  for (float j = _top; j <= _bottom; j += _width_cell_vertical)
      DrawLine(_left, j, _right, j, _width_line, _color);
}

}
