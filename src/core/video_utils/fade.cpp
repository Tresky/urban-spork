#include "../../utils/globals.hpp"
#include "../video_manager.hpp"

#include "fade.hpp"

namespace rpg_video
{

namespace private_video
{

Fader::Fader()
  : final_color(sf::Color::Transparent)
  , initial_color(sf::Color::Transparent)
  , current_color(sf::Color::Transparent)
  , elapsed_time(0.f)
  , total_time(0.f)
  , is_fading(false)
  , transition_fade(false)
{
  sf::Vector2f size = sf::Vector2f(VideoManager->GetScreenWidth(),
                                   VideoManager->GetScreenHeight());
  rect.setSize(size);
  rect.setFillColor(current_color);
}

void Fader::BeginFade(const sf::Color& _color,
                      const int _time,
                      const bool _trans)
{
  transition_fade = _trans;
  is_fading = true;

  total_time = _time;

  initial_color = current_color;
  final_color = _color;
  elapsed_time = 0.f;

  Update(0);
}

void Fader::Update(const int _elapsed_time)
{
  if (!IsFading())
    return;

  if (elapsed_time >= total_time)
  {
      current_color = final_color;
      is_fading = false;
      return;
  }

  float percent_complete = static_cast<float>(elapsed_time) / static_cast<float>(total_time);

  current_color = sf::Color(0, 0, 0, percent_complete);

  elapsed_time += _elapsed_time;
}

void Fader::Draw()
{
  VideoManager->DrawShape(rect);
}


}

}
