/**
 * \file globals.hpp
 *
 * Contains all includes and functionality
 * that is global across the entire project.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 03-12-16
 */

/**
 * \defgroup Utilities Utilities
 *
 * Package containing all of the extraneous
 * functionalities that are necessary across most of
 * the program. This includes things like:
 *  - Exceptions
 *  - Singletons
 *  - Lua Bridge
 *  - TermColor
 *  - Global Definitions
 */

/**
 * \defgroup SystemEngine SystemEngine
 *
 */

/**
 * \defgroup ModeEngine
 */

/**
 * \defgroup ScriptEngine
 */

/**
 * \defgroup VideoEngine VideoEngine
 */

// C++ Standard Libraries
#include <iostream>
  using std::cout;
  using std::cerr;
  using std::endl;
#include <iomanip>
  using std::setw;
#include <fstream>
  using std::ifstream;
  using std::ofstream;
#include <vector>
  using std::vector;
#include <stack>
  using std::stack;
#include <queue>
  using std::queue;
#include <string>
  using std::string;
  using std::to_string;
#include <map>
  using std::map;
#include <cmath>

// SFML Libraries
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Additional External Libraries
#include "termcolor.hpp"

// Custom Libraries
#include "../core/resources.hpp"
#include "exception.hpp"
  using rpg_utils::Exception;

// Environment Variables
#define DEBUG_MODE true

const string HOME_PATH = "~/Documents/playground/cpp_rpg";

// Helper functions designed to help in debugging
// and displaying error messages.
#define PRINT_DEBUG cout << termcolor::green << "DEBUG :: " << __FILE__ << " :: " << __FUNCTION__ << " :: " << __LINE__ << termcolor::reset << "\n  -- "
#define PRINT_WARNING cout << termcolor::yellow << "WARNING :: " << __FILE__ << " :: " << __FUNCTION__ << " :: " << __LINE__ << termcolor::reset << "\n  -- "
#define PRINT_ERROR cout << termcolor::red << "ERROR :: " << __FILE__ << " :: " << __FUNCTION__ << " :: " << __LINE__ << termcolor::reset << "\n  -- "

// Same as above, but with a conditional for evaluation.
#define IF_PRINT_DEBUG(var) if (var) cout << termcolor::green << "DEBUG :: " << __FILE__ << " :: " << __FUNCTION__ << " :: " << __LINE__ << termcolor::reset << "\n  -- "
#define IF_PRINT_WARNING(var) if (var) cout << termcolor::yellow << "WARNING :: " << __FILE__ << " :: " << __FUNCTION__ << " :: " << __LINE__ << termcolor::reset << "\n  -- "
