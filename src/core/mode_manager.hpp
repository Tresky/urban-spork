/**
 * mode_manager.hpp
 *
 * Contains a class representing a GameMode to be
 * inherited by other classes. Also, a singleton class
 * for managing all of the GameMode objects is
 * defined in this file.
 *
 * @author: Tyler Petresky <hello@tylerpetresky.com>
 * @date: 03-12-16
 */

#ifndef MODE_MANAGER_HPP
#define MODE_MANAGER_HPP

#include "../utils/singleton.hpp"

namespace rpg_mode_manager
{

class ModeEngine;

extern ModeEngine *ModeManager;
extern bool MODE_MANAGER_DEBUG;

 /**
  *
  */
class GameMode
{
  friend class ModeEngine;
public:
  /**
   * Default constructor
   */
  GameMode();

  /**
   * Destructor
   */
  virtual ~GameMode();

  /**
   * Update the GameMode object
   */
  virtual void Update();

  /**
   * Draw anything within the GameMode object.
   * This will be different for every child class.
   */
  virtual void Draw() = 0;

  /**
   * Resets the current game state to be reused
   */
  virtual void Reset() = 0;

private:
  /**
   * Delete the copy functions so they are not used.
   */
  GameMode(const GameMode& _copy) = delete;
  GameMode& operator=(GameMode& _copy) = delete;
};

/**
 *
 */
class ModeEngine : public rpg_utils::Singleton<ModeEngine>
{
  friend class rpg_utils::Singleton<ModeEngine>;
public:
  /**
   * Destructor
   */
  ~ModeEngine();

  /**
   * Inherited from Singleton; initialize the
   * ModeEngine singleton object.
   * @return True if successful, false otherwise
   */
  bool InitSingleton();

  /**
   * Pop a single GameMode off of the stack.
   * @param _fade_out True if you want to fade out
   * @param _fade_in  True if you want to fade into the next GameMode
   */
  void Pop(bool _fade_out = false, bool _fade_in = false);

  /**
   * Pop all of the GameModes from the stack.
   */
  void PopAll();

  /**
   * Push a single GameMode onto the stack.
   * @param _game_mode Pointer to GameMode to push
   * @param _fade_out  True if you want to fade out
   * @param _fade_in   True if you want to fade into the next GameMode
   */
  void Push(GameMode *_game_mode, bool _fade_out = false, bool _fade_in = false);

  /**
   * Returns pointer to the top GameMode (current)
   * @return Pointer to the current GameMode
   */
  GameMode *GetTop();

  /**
   * Update the current GameMode
   */
  void Update();

  /**
   * Draw the current GameMode
   */
  void Draw();

private:
  /**
   * Only instantiate one instance using rpg_utils::Singleton
   */
  ModeEngine();

  // Vector representing the official GameMode stack.
  // We use a vector just in case we ever want to print
  // a debug stack.
  vector<GameMode*> mode_stack;

  // Vector of the GameModes to be pushed during the next Update()
  vector<GameMode*> push_stack;

  // True if a state change has occurred and we need to
  // change the active GameMode
  bool state_change;

  // Number of GameModes to pop during the next Update()
  unsigned int pop_count;

  // Tells if there is a transitional fade in that needs to happen
  bool fade_in;
  bool fade_out;
  bool fade_out_finished;
};

} // namespace rpg_mode_manager

#endif // MODE_MANAGER_HPP
