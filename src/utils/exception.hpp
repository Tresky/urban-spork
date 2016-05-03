/**
 * \file exception.hpp
 *
 * This file holds a single class: Exception. The
 * file is contained within the Utilities package.
 */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

namespace std
{ class exception; }

// Utilities namespace
namespace rpg_utils
{

/**
 * \class Exception
 * \brief A class for catching and handling errors.
 *
 * \ingroup Utilities
 *
 * This class is used globally to catch and raise exceptions
 * whenever things go wrong. Most of the sub-systems try to deal
 * with their own errors, but in the event that it cannot be
 * handled at the local level, an instance of this class will be
 * raised so the program can safely try to handle it at a
 * higher level.
 *
 * \note Inherits from std::exception in the standard library.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 03-21-16
 */
class Exception : public std::exception
{
public:
  /**
   * Constructor
   * @param  _message C-Style string message
   */
  explicit Exception(const char* _message)
    : message(_message)
  {}

    /**
     * Constructor
     * @param  _message C++ Style string message
     */
  explicit Exception(const std::string& _message)
    : message(_message)
  {}

  /**
   * Destructor
   */
  virtual ~Exception() throw() {}

  /**
   * Returns the error message for this exception.
   * @return C-Style string error message
   */
  virtual const char* what() const throw()
  {
     return message.c_str();
  }

protected:
  //! Error message
  std::string message;
};

}

#endif // EXCEPTION_HPP
