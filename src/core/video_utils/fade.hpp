#ifndef FADE_HPP
#define FADE_HPP

namespace rpg_video
{

namespace private_video
{

class Fader
{
public:
  Fader();

  void BeginFade(const sf::Color& _color, const int _time, const bool _trans);

  void Update(const int _elapsed_time);

  bool IsFading()
  {
    return is_fading;
  }

  void StartTransitionFadeOut(const sf::Color& _color, const int _time)
  {
    BeginFade(_color, _time, true);
  }

  void TransitionalFadeIn(const int _time)
  {
    BeginFade(sf::Color::Transparent, _time, true);
  }

  void Draw();

private:
  sf::Color final_color;

  sf::Color initial_color;

  sf::Color current_color;

  int elapsed_time;

  int total_time;

  bool is_fading;

  bool transition_fade;

  sf::RectangleShape rect;
};

}

}

#endif // FADE_HPP
