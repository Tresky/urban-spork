#ifndef INPUT_HPP
#define INPUT_HPP

#include "../utils/singleton.hpp"

#include <SFML/Graphics.hpp>

namespace rpg_input
{

class InputEngine;
extern InputEngine* InputManager;
extern bool INPUT_DEBUG;

namespace private_input
{

struct KeyCode
{
  sf::Keyboard::Key up;
  sf::Keyboard::Key down;
  sf::Keyboard::Key left;
  sf::Keyboard::Key right;
};

}

class InputEngine : public rpg_utils::Singleton<InputEngine>
{
  friend class rpg_utils::Singleton<InputEngine>;
public:
  /**
   * Destructor
   */
  ~InputEngine();

  /**
   * Initialize the Singleton.
   * @return True if successful, false otherwise
   */
  bool InitSingleton();

  /**
   * Polls the SFML RenderWindow for SFML events
   * and deal with them appropriately (ie. flags, etc).
   */
  void CaptureEvents();

  /**
   * Capture any keyboard or mouse input that is
   * coming in live through the hardware. Whenever
   * input is received, the appropriate flags are set.
   */
  void CaptureInput();

  /**
   * Gets the state of the "up" key.
   * @return True if pressed, false otherwise
   */
  bool IsUpKeyPressed()
  {
    return up_key_state;
  }

  /**
   * Gets the state of the "down" key.
   * @return True if pressed, false otherwise
   */
  bool IsDownKeyPressed()
  {
    return down_key_state;
  }

  /**
   * Gets the state of the "left" key.
   * @return True if pressed, false otherwise
   */
  bool IsLeftKeyPressed()
  {
    return left_key_state;
  }

  /**
   * Gets the state of the "right" key.
   * @return True if pressed, false otherwise
   */
  bool IsRightKeyPressed()
  {
    return right_key_state;
  }

  bool NoMovementKeysPressed()
  {
    return !(right_key_state &&
             left_key_state &&
             up_key_state &&
             down_key_state);
  }

private:
  /**
   * Constructor
   */
  InputEngine();

  // SFML event object
  sf::Event event;

  // Object containing all of the key code definitions
  private_input::KeyCode key_codes;

  // Key state flags
  bool up_key_state;
  bool down_key_state;
  bool left_key_state;
  bool right_key_state;
};

}

#endif // INPUT_HPP
