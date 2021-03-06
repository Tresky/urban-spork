#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "../../utils/singleton.hpp"

namespace rpg_script
{ class ScriptEngine; }

namespace rpg_global
{

class GlobalEngine;
class GlobalCharacter;

// Singleton object
extern GlobalEngine* GlobalManager;
extern bool GLOBAL_DEBUG;

/**
 *
 */
class GlobalEngine : public rpg_utils::Singleton<GlobalEngine>
{
  friend class rpg_utils::Singleton<GlobalEngine>;
  friend class rpg_script::ScriptEngine;
public:
  /**
   * Destructor
   */
  ~GlobalEngine();

  /**
   * Initialize the singleton instance.
   * @return True if successful, false otherwise
   */
  bool InitSingleton();

  GlobalCharacter* GetCharacter(const unsigned int _id);

  GlobalCharacter* GetHero();

  void SetPreviousLocation(const std::string& _loc)
  {
    prev_loc = _loc;
  }

  std::string GetPreviousLocation()
  {
    return prev_loc;
  }

  void SetPreviousPosition(const int _x, const int _y)
  {
    prev_pos = sf::Vector2i(_x, _y);
  }

  int GetPreviousX() const
  {
    return prev_pos.x;
  }

  int GetPreviousY() const
  {
    return prev_pos.y;
  }

  bool AddCharacter(const GlobalCharacter* _char);

  void NewGame();

  void Update();

  void DrawGlobalCharacters();

private:
  GlobalEngine();

  std::map<signed int, GlobalCharacter*> global_characters;

  std::queue<int> reusable_ids;

  signed int counter;

  signed int hero_id;

  std::string prev_loc;

  sf::Vector2i prev_pos;

};

}

#endif // GLOBAL_HPP
