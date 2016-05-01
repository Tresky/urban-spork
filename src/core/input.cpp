#include "../utils/globals.hpp"
#include "input.hpp"

#include "video_manager.hpp"
#include "system.hpp"
#include "global/global.hpp"

namespace rpg_input
{

InputEngine* InputManager = nullptr;
bool INPUT_DEBUG = true;

InputEngine::InputEngine()
{
  IF_PRINT_DEBUG(INPUT_DEBUG) << "InputEngine constructor called" << endl;
}

InputEngine::~InputEngine()
{
  IF_PRINT_DEBUG(INPUT_DEBUG) << "InputEngine destructor called" << endl;
}

bool InputEngine::InitSingleton()
{
  // TODO: Load this from settings file
  key_codes.up = sf::Keyboard::W;
  key_codes.down = sf::Keyboard::S;
  key_codes.left = sf::Keyboard::A;
  key_codes.right = sf::Keyboard::D;

  return true;
}

void InputEngine::CaptureEvents()
{
  sf::RenderWindow* handle = rpg_video::VideoManager->GetWindowHandle();
  while (handle->pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      rpg_system::SystemManager->ExitGame();
  }
  handle = nullptr;
}

void InputEngine::CaptureInput()
{
  up_key_state = sf::Keyboard::isKeyPressed(key_codes.up);
  down_key_state = sf::Keyboard::isKeyPressed(key_codes.down);
  left_key_state = sf::Keyboard::isKeyPressed(key_codes.left);
  right_key_state = sf::Keyboard::isKeyPressed(key_codes.right);
}



}
