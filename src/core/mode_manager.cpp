#include "../utils/globals.hpp"
#include "video_manager.hpp"
#include "mode_manager.hpp"
#include "system.hpp"

using namespace rpg_utils;
using namespace rpg_system;

namespace rpg_mode_manager
{

ModeEngine *ModeManager = nullptr;
bool MODE_MANAGER_DEBUG = true;

/*****************************
 * GameMode Class Definition *
 *****************************/
GameMode::GameMode()
{
  IF_PRINT_DEBUG(MODE_MANAGER_DEBUG) << "GameMode constructor called" << endl;
}

GameMode::~GameMode()
{
  IF_PRINT_DEBUG(MODE_MANAGER_DEBUG) << "GameMode destructor called" << endl;
}

void GameMode::Update()
{

}

/********************************
 * ModeEngine Class Definitions *
 ********************************/
ModeEngine::ModeEngine()
  : state_change(false)
  , pop_count(0)
  , fade_in(false)
  , fade_out(false)
  , fade_out_finished(false)
{
  IF_PRINT_DEBUG(MODE_MANAGER_DEBUG) << "ModeEngine constructor called" << endl;
}

ModeEngine::~ModeEngine()
{
  IF_PRINT_DEBUG(MODE_MANAGER_DEBUG) << "ModeEngine destructor called" << endl;

  // Delete any game modes on the stack
  while (!mode_stack.empty())
  {
    delete mode_stack.back();
    mode_stack.pop_back();
  }

  // Delete any game modes on the push stack
  while (!push_stack.empty())
  {
    delete push_stack.back();
    push_stack.pop_back();
  }
}

bool ModeEngine::InitSingleton()
{
    // Delete any game modes on the stack
    while(!mode_stack.empty())
    {
        delete mode_stack.back();
        mode_stack.pop_back();
    }

    // Delete any game modes on the push stack
    while(!push_stack.empty())
    {
        delete push_stack.back();
        push_stack.pop_back();
    }

    // Reset the pop counter
    pop_count = 0;

    return true;
}

void ModeEngine::Pop(bool _fade_out, bool _fade_in)
{
  fade_in = _fade_in;

  pop_count++;
  state_change = true;

  if (_fade_out)
  {
    rpg_video::VideoManager->StartTransitionFadeOut(sf::Color::Black, 500);
    fade_out = true;
    fade_out_finished = false;
  }
  else
  {
    fade_out = false;
    fade_out_finished = true;
  }
}

void ModeEngine::PopAll()
{
  pop_count = static_cast<unsigned int>(mode_stack.size());
}

void ModeEngine::Push(GameMode *_game_mode, bool _fade_out, bool _fade_in)
{
  push_stack.push_back(_game_mode);

  state_change = true;

  fade_in = _fade_in;

  if(_fade_out)
  {
    rpg_video::VideoManager->StartTransitionFadeOut(sf::Color::Black, 750);
    fade_out = true;
    fade_out_finished = false;
  }
  else
  {
    fade_out = false;
    fade_out_finished = true;
  }
}

GameMode* ModeEngine::GetTop()
{
  if (mode_stack.empty())
    return nullptr;
  return mode_stack.back();
}

void ModeEngine::Update()
{
  // Check whether the fade out is done.
  if (fade_out && !rpg_video::VideoManager->IsFading())
  {
      fade_out = false;
      fade_out_finished = true;
  }

  // If a Push() or Pop() function was called, we need to adjust the state of the game stack.
  if(fade_out_finished && state_change)
  {
    // Pop however many game modes we need to from the top of the stack
    while(pop_count != 0)
    {
      if (mode_stack.empty())
      {
        PRINT_WARNING << "Tried to pop off more game modes than were on the stack!" << std::endl;
        pop_count = 0;
        break;
      }
      delete mode_stack.back();
      mode_stack.pop_back();
      pop_count--;
    }

    // Push any new game modes onto the true game stack.
    while(!push_stack.empty())
    {
      // Tell the previous game mode about being deactivated.
      if(!mode_stack.empty() && mode_stack.back())
        mode_stack.back()->Deactivate();

      mode_stack.push_back(push_stack.back());
      push_stack.pop_back();
    }

    // Make sure there is a game mode on the stack, otherwise we'll get a segmentation fault.
    if(mode_stack.empty())
    {
      PRINT_WARNING << "game stack is empty, exiting application" << std::endl;
      SystemManager->ExitGame();
    }

    // Call the newly active game mode's Reset() function to re-initialize the game mode
    mode_stack.back()->Reset();

    // Reset the state change variable
    state_change = false;

    // Reset the fade out member
    fade_out_finished = false;

    // We can now fade in, or not
    if (fade_in)
      rpg_video::VideoManager->TransitionalFadeIn(750);

    // Call the system manager and tell it that the active game mode changed so it can update timers accordingly
    SystemManager->ExamineSystemTimers();

    // Re-initialize the game update timer so that the new active game mode does not begin with any update time to process
    SystemManager->InitUpdateTimer();
  }

  // Call the Update function on the top stack mode (the active game mode)
  if(!mode_stack.empty())
    mode_stack.back()->Update();
}

void ModeEngine::Draw()
{
  if(mode_stack.empty())
    return;

  mode_stack.back()->Draw();
}

}
