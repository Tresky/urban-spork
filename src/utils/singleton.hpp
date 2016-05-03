/**
 * \file singleton.hpp
 *
 * Contains a templated class representing the
 * Singleton programming pattern. This will used
 * to create multiple "controllers" for various
 * aspects of the game.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 03-12-16
 */

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

// Utilities namespace
namespace rpg_utils
{

 /**
  * \class Singleton
  * \brief Templated class for creating singletons.
  *
  * \ingroup Utilities
  *
  * A <b>singleton</b> is a programming pattern that allows you to
  * instantiate a class, but only once. The class can only be
  * instantiated via a public member, <i>CreateSingleton()</i>, and
  * will only create its instance in a static member.
  *
  * In order to create a singleton in this project, simply create
  * the class that you want to be a singleton, then have it inherit
  * from this class. Then you must define the virtual function
  * <i>InitSingleton()</i> in your new class and initialize the static member
  * to nullptr. See any of the other singletons for examples.
  *
  * To use the created singleton see the example below:
  *
  * <b>Singleton Definition File</b>
  * \code
  * // ------------------------
  * // Header File: example.hpp
  * // ------------------------
  *
  * #include "singleton.hpp" // Include base class
  *
  * class Example; // Declare class
  * extern Example* ExampleSingleton; // Declare instance of singleton
  *
  * class Example: public Singleton<Example>
  * {
  * 	friend class Singleton<Example>;
  *
  * public:
  * 	~Example() {}
  *
  * 	bool InitSingleton()
  * 	{
  * 		// Code here
  * 	}
  *
  * private:
  * 	Example(); // Private constructor
  * };
  *
  * // ------------------------
  * // Source File: example.cpp
  * // ------------------------
  *
  * // Set initial value of instance variable
  * Example* ExampleSingleton = nullptr;
  *
  * // Class definitions go here...
  * \endcode
  *
  * <b>Singleton Creation/Destruction Example</b>
  * \code
  * // main.cpp
  * #include "example.hpp"
  *
  * // do some stuff
  *
  * ExampleSingleton = Example::CreateSingleton();
  *
  * if (!ExampleSingleton->InitSingleton())
  * 	// Handle error
  *
  * // do some stuff
  *
  * Example::DestroySingleton();
  * \endcode
  *
  * \author Tyler Petresky <hello@tylerpetresky.com>
  * \date 03-12-16
  */
template<typename T> class Singleton
{
protected:
  //! Static pointer of T type to represent the single instance
  //! of the singleton
  static T *_instance;

  //! Protected default constructor. This will inherit as private
  //! and so will never be accessible to use. To construct the
  //! singleton, see CreateSingleton() and InitSingleton().
  //!
  //! \see CreateSingleton()
  //! \see InitSingleton()
  Singleton() {}

  //! Protected default destructor. This will inherit as private
  //! and so will never be accessible to use. To deconstruct the
  //! singleton, see DestroySingleton().
  //!
  //! \see DestroySingleton().
  virtual ~Singleton() {}

public:
  /**
   * Creates the Singleton instance as a static
   * variable within the class. This function is static
   * and cannot be redefined in child classes.
   *
   * \note This function should be called before doing anything
   * with your singleton.
   *
   * @return Pointer to new Singleton instance
   */
  static T* CreateSingleton()
  {
    if (_instance == nullptr)
      _instance = new T();
    return _instance;
  }

  /**
   * Free the memory associated with the Singleton object
   * and sets the instance pointer to nullptr. This function
   * is static and cannot be redefined in child classes.
   *
   * \note This function should be the final one called
   * before the program terminates.
   */
  static void DestroySingleton()
  {
    if (_instance != nullptr)
      delete _instance;
    _instance = nullptr;
  }

  /**
   * Returns a pointer to the static Singleton instance. This
   * function is static and cannot be redefined in child classes.
   *
   * @return Pointer to Singleton instance
   */
  static const T* GetSingleton()
  {
    return _instance;
  }

  /**
   * Purely virtual function to initialize the Singleton object.
   * This function should be defined in each child class of this
   * class. This function will act as the initial setup function
   * since the constructor of a singleton cannot be called; this
   * takes the place of a constructor.
   *
   * @return True if successful, false otherwise
   */
  virtual bool InitSingleton() = 0;

private:
  /**
   * Delete the copy constructor and the assignment
   * operator. This prevents them from being used to
   * create a second instance of a Singleton object.
   */
  Singleton(const Singleton& _copy) = delete;
  Singleton& operator=(const Singleton& _copy) = delete;
};

template<typename T> T* Singleton<T>::_instance = nullptr;

} // namespace utils

#endif //SINGLETON_HPP
