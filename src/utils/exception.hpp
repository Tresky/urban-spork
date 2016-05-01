#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

namespace std
{ class exception; }

namespace rpg_utils
{

/**
 *
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
   * Returns a pointer to the error message.
   * @return Pointer to a C-Style error message
   */
  virtual const char* what() const throw()
  {
     return message.c_str();
  }

protected:
  // Error message
  std::string message;
};

}

#endif // EXCEPTION_HPP
