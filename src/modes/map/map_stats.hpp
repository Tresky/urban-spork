#ifndef MAP_STATS_HPP
#define MAP_STATS_HPP

namespace rpg_map_mode
{

namespace private_map_mode
{

class Statistics
{
public:
  Statistics() {}

  ~Statistics() {}

  int GetCurrentHealth()
  {
    return health;
  }

  int GetMaxHealth()
  {
    return max_health;
  }

  float GetPercentageHealth()
  {
    return static_cast<float>(health) / static_cast<float>(max_health);
  }

  void SetCurrentHealth(const int _health)
  {
    health = _health;
  }

  void SetMaxHealth(const int _health)
  {
    max_health = _health;
  }

private:
  int health;
  int max_health;
};

}

}

#endif // MAP_STATS_HPP
