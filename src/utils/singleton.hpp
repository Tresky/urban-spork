/**
 * singleton.hpp
 *
 * Contains a templated class representing the
 * Singleton programming pattern. This will used
 * to create multiple "controllers" for various
 * aspects of the game.
 *
 * @author: Tyler Petresky <hello@tylerpetresky.com>
 * @date: 03-12-16
 */

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace rpg_utils
{

/**
 * @class: Singleton
 *
 * Represents a Singleton programming pattern for
 * inheriting by other classes.
 */
template<typename T> class Singleton
{
protected:
  static T *_instance;

  Singleton() {}

  virtual ~Singleton() {}

public:
  /**
   * Creates the Singleton instance as a static
   * variable within the class.
   * @return Pointer to Singleton instance
   */
  static T* CreateSingleton()
  {
    if (_instance == nullptr)
      _instance = new T();
    return _instance;
  }

  /**
   * Free the memory associated with the Singleton
   * object and sets the instance to nullptr.
   */
  static void DestroySingleton()
  {
    if (_instance != nullptr)
      delete _instance;
    _instance = nullptr;
  }

  /**
   * Returns the static Singleton instance.
   * @return Pointer to Singleton instance
   */
  static const T* GetSingleton()
  {
    return _instance;
  }

  /**
   * Virtual function to initialize the Singleton
   * object. This will be specific to the type of
   * Singleton being created.
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
