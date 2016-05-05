#ifndef GLOBAL_ACTORS_HPP
#define GLOBAL_ACTORS_HPP

#include "../image.hpp"

namespace rpg_video
{ class Image; }

namespace rpg_global
{

class GlobalCharacter
{
public:
  GlobalCharacter()
    : resource_id(-1)
    , current_animation("")
    , direction("")
  {}

  ~GlobalCharacter()
  {
    // Need to delete animations
  }

  bool LoadAnimations(const std::string& _filepath);

  void SetCurrentAnimation(const std::string& _key);

  void SetDirection(const std::string& _direction);

  std::string GetDirection();

  void SetMapImage(const unsigned int _id);

  rpg_video::Image* GetMapImage() const;

  sf::Vector2f GetPosition();

  void SetPosition(const int _x, const int _y);

  void Move(const int _x, const int _y);

  void Update();

  void Draw();

private:
  int resource_id;

  std::string direction;

  sf::Vector2f position;

  map<std::string, rpg_video::AnimatedImage*> animations;

  std::string current_animation;
};

}

#endif // GLOBAL_ACTORS_HPP
