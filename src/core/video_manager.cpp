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

}
