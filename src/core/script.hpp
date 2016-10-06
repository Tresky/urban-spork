/**
 * \file script.hpp
 *
 * Contains a singleton class for managing all
 * scripts that are loaded in the game. This file
 * also contains a base-class that serves as a
 * parent ReadScript and WriteScript which are
 * used to read and write from/to Lua scripts
 * respectively.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 06-18-16
 */

#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "../utils/singleton.hpp"

// Lua Resources
extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}
#include "../utils/sol2.hpp"

namespace rpg_script
{

/**
 * \brief Tiny structure that denotes a format
 * for defining errors that occur during
 * the reading and writing of our Lua scripts.
 * Only used inside of Script.
 *
 * \ingroup ScriptEngine
 */
struct ScriptError
{
  enum ErrorType
  {
    NO_ERROR = -1,
    FILE_NOT_FOUND = 0,
    FILE_NOT_OPENED = 1,
    FILE_ALREADY_OPEN = 2,
    DATA_NOT_FOUND = 3,
    DATA_NOT_RETRIEVED = 4,
    TABLE_NOT_FOUND = 5,
    FUNCTION_FAILED = 6
  };

  //! Type of error that occured; error code
  ErrorType type;

  //! Error message
  std::string message;
};

class ScriptEngine;
extern ScriptEngine* ScriptManager;
extern bool SCRIPT_DEBUG;

 /**
  * \class Script
  * \brief Base class that is inherited by ReadScript and
  * WriteScript for providing a common interface for being
  * able to store scripts.
  *
  * \ingroup ScriptEngine
  *
  * This class serves two main purposes:
  * 	- Provide a common interface for storing ReadScripts
  * 		and WriteScripts; namely polymorphism.
  * 	- Provide a common error handling method for both
  * 		ReadScripts and WriteScripts.
  *
  * As this class is purely-abstract, it cannot be instantiated.
  * Any attempt to do so will generate a compiler error.
  *
  * \author Tyler Petresky <hello@tylerpetresky.com>
  * \date 06-18-16
  */
class Script
{
public:
  /**
   * Default constructor
   */
  Script()
    : is_opened(false)
  {}

  /**
   * Destructor
   */
  virtual ~Script() {}

  /**
   * Virtual function to open a file using the Lua state.
   * \param  _filename String with filename to open
   * \return True if successful, false otherwise
   */
  virtual bool OpenFile(const std::string& _filename) = 0;

  /**
   * Closes the currently open file. This is not defined
   * for this class because closing the file might be
   * different depending on use case.
   */
  virtual void CloseFile() = 0;

  /**
   * Returns whether or not a file is currently open.
   * \return True if open, false otherwise
   */
  bool IsOpen() const
  {
    return is_opened;
  }

  /**
   * Returns the filename of the script that is
   * currently open for reading/writing.
   * \return String containing filename
   */
  const std::string GetFilename()
  {
    return filename;
  }

  /**
   * Returns whether or not the current script has
   * encountered any errors.
   *
   * \note This system will not alert you about errors
   * until you check and see if there are any and then
   * print them with HasError().
   *
   * \see HasError()
   *
   * \return True if the script has errors, false otherwise
   */
  bool HasError() const
  {
    return !errors.empty();
  }

  /**
   * Returns the next error that was encountered. All
   * errors that are encountered are stored in a FIFO
   * queue. This function will return the first error
   * and pop it off of the queue.
   *
   * \note Once an error has been popped from the queue,
   * it will be popped and deleted. Make sure to deal
   * with errors as you encounter them.
   *
   * \return ScriptError object representing the error encountered
   */
  ScriptError GetNextError()
  {
    ScriptError error = errors.front();
    errors.pop();
    return error;
  }

  /**
   * Prints all of the errors that were encountered
   * while reading/writing the Lua script.
   *
   * \note This will pop *all* of the errors from
   * the error queue and print them. That means you
   * will have no opportunity to deal with the errors
   * gracefully. This if really only useful for
   * debugging purposes. To view errors one-by-one,
   * see GetNextError().
   *
   * \see GetNextError()
   */
  void PrintErrors()
  {
    while (!errors.empty())
    {
      ScriptError err = GetNextError();
      PRINT_WARNING << "ScriptError[" << err.type << "]: " << err.message << endl;
    }
  }

protected:
  //! Lua state to load with; this is copied from ScriptManager
  sol::state* L;

  //! Filename of the Lua script being used
  std::string filename;

  //! Boolean flag stating whether a file is open or not
  bool is_opened;

  //! FIFO queue for storing all errors that are encountered.
  std::queue<ScriptError> errors;
}; // class Script

/**
 * \class ScriptEngine
 * \brief Global singleton class that manages all of
 * the Script objects throughout the game.
 *
 * \ingroup ScriptEngine
 *
 * This Singleton is very simple and acts as a container
 * for all of the Script objects that are used throughout the game.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 06-18-16
 */
class ScriptEngine : public rpg_utils::Singleton<ScriptEngine>
{
  friend class rpg_utils::Singleton<ScriptEngine>;
  friend class Script;
public:
  /**
   * Destructor
   */
  ~ScriptEngine();

  /**
   * Inherited from Singleton; initialize the
   * ScriptManager singleton object.
   * \return True if successful, false otherwise
   */
  bool InitSingleton();

  /**
   * Returns the global Lua state.
   * \return Pointer to the global Lua state object
   */
  sol::state* GetGlobalState()
  {
    return L;
  }

private:
  /**
   * Constructor only called once from Singleton.
   */
  ScriptEngine();

  //! Lua state object
  sol::state* L;

  //! Hash-map that maps all of the open Lua scripts to their filenames as keys
  std::map<std::string, Script*> open_scripts;
}; // class ScriptEngine

} // namespace rpg_script

#endif // SCRIPT_HPP
