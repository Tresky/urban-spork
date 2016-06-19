#include "../utils/globals.hpp"
#include "video_manager.hpp"
#include "video_utils/fade.hpp"

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
  , debug(true)
  , screen_fader(nullptr)
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

VideoEngine::~VideoEngine()
{
  IF_PRINT_DEBUG(VIDEO_DEBUG) << "VideoEngine destructor called" << endl;

  if (window->isOpen())
    window->close();
  if (window)
    delete window;
  if (screen_fader)
    delete screen_fader;
}

void VideoEngine::CreateWindow(unsigned int _width, unsigned int _height, string _title)
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

  if (!screen_fader)
    screen_fader = new private_video::Fader();
}

void VideoEngine::Update()
{
  if (screen_fader)
    screen_fader->Update(SystemManager->GetUpdateTime());

  UpdateFPS();
}

void VideoEngine::DrawDebugInfo() const
{
  DrawFPS();
}

void VideoEngine::DrawSprite(sf::Sprite& _sprite) const
{
  window->draw(_sprite);
}

void VideoEngine::DrawShape(sf::Shape& _shape) const
{
  window->draw(_shape);
}

void VideoEngine::Display() const
{
  window->display();
}

void VideoEngine::Close() const
{
  window->close();
}

void VideoEngine::UpdateFPS()
{
  int current_fps = 1000 / SystemManager->GetUpdateTime();
  fps_text.setString("FPS: " + std::to_string(current_fps));
}

void VideoEngine::DrawFPS() const
{
  window->draw(fps_text);
}

bool VideoEngine::IsFading()
{
  return screen_fader->IsFading();
}

void VideoEngine::StartTransitionFadeOut(const sf::Color _color, const int _duration)
{
  screen_fader->StartTransitionFadeOut(_color, _duration);
}

void VideoEngine::TransitionalFadeIn(const int _duration)
{
  screen_fader->TransitionalFadeIn(_duration);
}

void VideoEngine::DrawFadeEffect()
{
    screen_fader->Draw();
}

void VideoEngine::DrawLine(const int _x1, const int _y1,
                           const int _x2, const int _y2,
                           const int _width, const sf::Color& _color) const
{
  sf::VertexArray line(sf::Lines, 2);
  line[0].position = sf::Vector2f(_x1, _y1);
  line[1].position = sf::Vector2f(_x2, _y2);

  line[0].color = _color;
  line[1].color = _color;

  window->draw(line);
}

void VideoEngine::DrawGrid(const float _left, const float _top,
                           const float _right, const float _bottom,
                           const float _width_cell,
                           const float _height_cell,
                           const unsigned _width_line, const sf::Color& _color) const
{
  // Draw the grid's vertical lines.
  for (float i = _left; i <= _right; i += _width_cell)
      DrawLine(i, _top, i, _bottom, _width_line, _color);

  // Draw the grid's horizontal lines.
  for (float j = _top; j <= _bottom; j += _height_cell)
      DrawLine(_left, j, _right, j, _width_line, _color);
}

}
