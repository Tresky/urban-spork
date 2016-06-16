/**
 * \file system.hpp
 *
 * Contains a singleton class for managing all
 * timers that are used in the game. This file
 * also contains a large class that is used to
 * create SystemTimer objects for timing the
 * various functions within the game such as
 * battle timers, day/night cycles, etc.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 06-14-16
 */

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "../utils/singleton.hpp"

#include "mode_manager.hpp"

namespace rpg_system
{

class SystemEngine;
extern SystemEngine *SystemManager;
extern bool SYSTEM_DEBUG;

const unsigned int SYSTEM_INFINITE_TIME = 0xFFFFFFFF;
const unsigned int SYSTEM_TIMER_NO_LOOPS = 0;
const unsigned int SYSTEM_TIMER_INFINITE_LOOP = -1;

/**
 * All of the possible states that the
 * SystemTimer class can be in.
 */
enum TimerStates
{
  TIMER_INVALID = -1,
  TIMER_INITIAL = 0,
  TIMER_RUNNING = 1,
  TIMER_PAUSED = 2,
  TIMER_FINISHED = 3
};

/**
 * \class SystemTimer
 * \brief Single class that is used to create timers
 * throughout the game for timing various functions.
 *
 * \ingroup SystemEngine
 *
 * This large class contains all of the functionality needed
 * to create, initialize, start, stop, pause, and all around
 * use timers throughout the game. These timers are global in
 * nature in the sense that they are very general purpose.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 06-14-16
 */
class SystemTimer
{
public:
  /**
   * Default constructor
   */
  SystemTimer();

  /**
   * Constructor for SystemTimer
   * \param _duration Integer for the length of the timer (milliseconds)
   * \param _loops    Integer for the number of loops to execute
   */
  SystemTimer(const unsigned int _duration, const unsigned int _loops = 0);

  /**
   * Destructor
   */
  virtual ~SystemTimer();

  /**
   * Initialize the timer to it's initial state.
   * \param _duration Integer for the length of the timer (milliseconds)
   * \param _loops    Intefer for the number of loops to execute
   */
  void InitTimer(const unsigned int _duration, const unsigned int _loops = 0);

  /**
   * Update the timer.
   */
  virtual void Update();
  virtual void Update(const unsigned int _time);

  /**
   * Reset the timer to it's initial state.
   */
  virtual void Reset()
  {
    if (timer_state != TimerStates::TIMER_INVALID)
    {
      timer_state = TIMER_INITIAL;
      time_expired = 0;
      times_completed = 0;
    }
  }

  /**
   * Set the timer's state to "running".
   */
  void Run()
  {
    if (IsInitial() || IsPaused())
      timer_state = TIMER_RUNNING;
  }

  /**
   * Set the timer's state to "paused".
   */
  void Pause()
  {
    if (IsRunning())
      timer_state = TIMER_PAUSED;
  }

  /**
   * Set the timer's state to "finished".
   */
  void Finish()
  {
    timer_state = TIMER_FINISHED;
  }

  /**
   * Returns boolean representing if the timer is
   * in it's initial state.
   * \return True if timer_state is TIMER_INITIAL, false otherwise
   */
  bool IsInitial() const
  {
    return (timer_state == TIMER_INITIAL);
  }

  /**
   * Returns boolean representing if the timer is
   * in it's running state.
   * \return True if timer_state is TIMER_RUNNING, false otherwise
   */
  bool IsRunning() const
  {
    return (timer_state == TIMER_RUNNING);
  }

  /**
   * Returns boolean representing if the timer is
   * in it's paused state.
   * \return True if timer_state is TIMER_PAUSED, false otherwise
   */
  bool IsPaused() const
  {
    return (timer_state == TIMER_PAUSED);
  }

  /**
   * Returns boolean representing if the timer is
   * in it's finished state.
   * \return True if timer_state if TIMER_FINISHED, false otherwise
   */
  bool IsFinished() const
  {
    return (timer_state == TIMER_FINISHED);
  }

  /**
   * Returns the timer that is left on the timer.
   * \return Unsigned integer representing the time left on the timer (milliseconds)
   */
  unsigned int TimeLeft() const
  {
    return (duration - time_expired);
  }

  float PercentComplete() const
  {
    switch(timer_state) {
    case TIMER_INITIAL:
        return 0.0f;
    case TIMER_RUNNING:
    case TIMER_PAUSED:
        return static_cast<float>(time_expired) / static_cast<float>(duration);
    case TIMER_FINISHED:
        return 1.0f;
    default:
        return 0.0f;
    }
  }

  /**
   * Set the duration of the timer.
   * \param _duration Unsigned integer to set the duration to (milliseconds)
   */
  void SetDuration(const unsigned int _duration);

  /**
   * Set the current expired time of the timer.
   * \param _time_expired Unsigned integer to set the expired time to (milliseconds)
   */
  void SetTimeExpired(const unsigned int _time_expired);

  /**
   * Set the number of times to loop the timer.
   * \param _loops Unsigned integer to set the number of loops to
   */
  void SetNumberOfLoops(const unsigned int _loops);

  /**
   * Set the mode owner of this timer.
   * \param _owner Pointer to GameMode to own this timer
   */
  void SetModeOwner(rpg_mode_manager::GameMode *_owner);

  /**
   * Returns the current state of the timer.
   * \return TimerState enumerator value representing the timer state
   */
  TimerStates GetState() const
  {
    return timer_state;
  }

  /**
   * Returns the duration of the timer.
   * \return Unsigned integer for the duration of the timer (milliseconds)
   */
  unsigned int GetDuration() const
  {
    return duration;
  }

  /**
   * Returns the time expired on the timer.
   * \return Unsigned integer for the time expired (milliseconds)
   */
  unsigned int GetTimeExpired() const
  {
    return time_expired;
  }

  /**
   * Returns the mode owner for this timer.
   * \return Pointer to mode owner, nullptr if none
   */
  rpg_mode_manager::GameMode *GetModeOwner() const
  {
    return mode_owner;
  }

  /**
   * Returns the number of loops currently completed.
   * \return Unsigned integer for the number of loops that have been completed
   */
  unsigned int GetTimesCompleted() const
  {
    return times_completed;
  }

protected:
  /**
   * Performs the actual updating of the class members.
   * \param _time Unsigned integer to update the timer by
   */
  void UpdateTimer(const unsigned int _time);

  //! Enumerator holding the current timer state
  TimerStates timer_state;

  //! Total duration of the timer in milliseconds
  unsigned int duration;

  //! Total number of loops to be completed
  signed int loops;

  //! Total time expired in the current loop in milliseconds
  unsigned int time_expired;

  //! Total number of loops completed since starting the timer
  unsigned int times_completed;

  //! Pointer to the game mode that owns this timer, nullptr if none
  rpg_mode_manager::GameMode* mode_owner;
};

/**
 * \class SystemEngine
 * \brief Global singleton class that manages all of
 * the SystemTimers throughout the game.
 *
 * \ingroup SystemEngine
 *
 * This Singleton class is very simple and only organizes
 * and handles the SystemTimers within the application.
 * Alongside this, there are a few smaller functionalities
 * that really didn't fit into any other system.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 06-14-16
 */
class SystemEngine : public rpg_utils::Singleton<SystemEngine>
{
  friend class rpg_utils::Singleton<SystemEngine>;
public:
  /**
   * Destructor
   */
  ~SystemEngine();

  /**
   * Inherited from Singleton; initialize the
   * SystemManager singleton object.
   * \return True if successful, false otherwise
   */
  bool InitSingleton();

  /**
   * Initializes all of the game's timers. This
   * function is called once in the main() function.
   */
  void InitGameTimers();

  /**
   * Initialize the update timer
   */
  void InitUpdateTimer()
  {
    last_update = global_time.getElapsedTime().asMilliseconds();
    update_time = 1;
  }

  /**
   * Updates the timers in the manager. This should
   * be called once in the main loop.
   */
  void UpdateTimers();

  /**
   * Checks all system timers and pauses or resumes them based
   * on active game modes.
   */
  void ExamineSystemTimers();

  /**
   * Returns the times since the last update has happened.
   * \return Unsigned integer for elapsed milliseconds since last update
   */
  inline unsigned int GetUpdateTime() const
  {
    return update_time;
  }

  /**
   * Set's the current playtime. This is to be used
   * whenever a save game is loaded from memory.
   * \param _hour   Integer representing number of hours
   * \param _minute Integer representing number of minutes
   * \param _second Integer representing number of seconds
   */
  void SetPlayTime(const int _hour, const int _minute, const int _second)
  {
    hours_played = _hour;
    minutes_played = _minute;
    seconds_played = _second;
    milliseconds_played = 0;
  }

  /**
   * Returns the number of hours played.
   * \return Integer for number of hours played
   */
  int GetPlayHours() const
  {
    return hours_played;
  }

  /**
   * Returns the number of minutes played.
   * \return Integer for number of minutes played
   */
  int GetPlayMinutes() const
  {
    return minutes_played;
  }

  /**
   * Returns the number of seconds played.
   * \return Integer for number of seconds played
   */
  int GetPlaySeconds() const
  {
    return seconds_played;
  }

  /**
   * Returns boolean representing if the user wants
   * to exit the game.
   * \return True if game is exiting, false otherwise
   */
  bool IsExiting() const
  {
    return is_exiting;
  }

  /**
   * Set flag to exit the game.
   */
  void ExitGame()
  {
    is_exiting = true;
  }

private:
  SystemEngine();

  //! Milliseconds since the last call to UpdateTimers()
  unsigned int last_update;

  //! Number of milliseconds that have transpired since the last timer update
  unsigned int update_time;

  //! Numbers of hours played by the player
  int hours_played;
  //! Number of minutes played by the player
  int minutes_played;
  //! Number of seconds played by the player
  int seconds_played;
  //! Number of milliseconds played by the player
  int milliseconds_played;

  //! Boolean flag used for signaling the exiting of the game
  bool is_exiting;

  //! SFML clock object to track the time since execution
  sf::Clock global_time;
}; // class SystemEngine

} // namespace rpg_system

#endif // SYSTEM_HPP
