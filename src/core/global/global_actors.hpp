#ifndef GLOBAL_ACTORS_HPP
#define GLOBAL_ACTORS_HPP

namespace rpg_video
{ class Image; }

namespace rpg_global
{

class GlobalCharacter
{
public:
  GlobalCharacter()
    : resource_id(-1)
  {}

  ~GlobalCharacter()
  {}

  void SetMapImage(const unsigned int _id);

  rpg_video::Image* GetMapImage() const;

  sf::Vector2f GetPosition();

  void SetPosition(const int _x, const int _y);

  void Move(const int _x, const int _y);

  void Draw();

private:
  int resource_id;

  sf::Vector2f position;
};

}

#endif // GLOBAL_ACTORS_HPP
