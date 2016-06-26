/**
 * \file map_utils.hpp
 *
 * This file houses any smaller, extraneous classes that
 * are useful to the Map Mode. This includes things like:
 *  - MapRectangle (useful for collision detection)
 *  - MapFrame (defines the parameters associated with the camera)
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 03-22-16
 */

#ifndef MAP_UTILS_HPP
#define MAP_UTILS_HPP

namespace rpg_map_mode
{

namespace private_map_mode
{

//! Various types of collisions between MapObjects
enum CollisionType {
    NO_COLLISION = 0,
    CHARACTER_COLLISION = 1,
    ENEMY_COLLISION = 2,
    WALL_COLLISION = 3
};
const int ALL_COLLISION = CHARACTER_COLLISION | ENEMY_COLLISION | WALL_COLLISION;

//! Various types of MapObject types
enum ObjectType {
  OBJECT_TYPE   = -1, //! Default type
  PHYSICAL_TYPE = 0,  //! This is a physical (visible) object, like a rock, a door, ect...
  VIRTUAL_TYPE  = 1,  //! This is used as another type of NPC sprite, but without interaction.
  SPRITE_TYPE   = 2,  //! This is a NPC sprite. Blocking for the hero, but not for the other sprites.
  ENEMY_TYPE    = 3  //! This is an enemy sprite. Blocking the hero, and triggering a battle.
};

/**
 * \class MapRectangle
 * \brief Very small class to represent a rectangular area of a map.
 * Primarily used for collision detection.
 *
 * \ingroup ModeEngine
 *
 * This class is a very small class that is used, primarily, for
 * collision detection between two objects. For an example of it being used,
 * check out the MapMode::Update() function. The only function available
 * to this class is a static function called CheckIntersection() which returns
 * a boolean value indicating whether two MapRectangles are interecting or not.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 03-22-16
 */
class MapRectangle
{
public:
  /**
   * Default constructor. Creates an empty MapRectangle.
   */
  MapRectangle()
    : left(0.f)
    , top(0.f)
    , right(0.f)
    , bottom(0.f)
  {}

  /**
   * Constructor. Creates a MapRectangle based on passed parameters.
   *
   * \param _left   Float defining the left side of rectangle
   * \param _top    Float defining the top side of the rectangle
   * \param _right  Float defining the right side of the rectangle
   * \param _bottom Float defining the bottom side of the rectangle
   */
  MapRectangle(const float _left, const float _top,
               const float _right, const float _bottom)
    : left(_left)
    , top(_top)
    , right(_right)
    , bottom(_bottom)
  {}

  //! X-value of the left of the rectangle
  float left;
  //! Y-value of the top of the rectangle
  float top;
  //! X-value of the right of the rectangle
  float right;
  //! Y-value of the bottom of the rectangle
  float bottom;

  /**
   * Static method to check for intersection between two objects
   * of the type, MapRectangle. This is used extensively for
   * collision detection.
   *
   * \note Parameter order does not matter.
   *
   * \param  _one First rectangle to compare
   * \param  _two Second rectangle to compare
   * \return True if _one and _two are intersecting, false otherwise
   */
  static bool CheckIntersection(const MapRectangle& _one, const MapRectangle& _two);

  /**
   * Static method to check if two MapRectangles are within a specific
   * range of each other. The measurements for this function are taken
   * from opposing edges. e.g. _one.left is compared to _two.right.
   *
   * \note Parameter order does not matter.
   *
   * \param  _one   First rectangle to compare
   * \param  _two   Second rectangle to compare
   * \param  _range Distance to check between the rectangles
   * \return True if _one and _two are within _range pixels, false otherwise
   */
  static bool InRange(const MapRectangle& _one, const MapRectangle& _two, const int _range);
}; // class MapRectangle



/**
 * \class MapFrame
 * \brief Small class that defines the dimensions and
 * properties of the camera.
 *
 * \ingroup ModeEngine
 *
 * This class is used to define the dimensions and properties of
 * the camera that is drawing the current map. We use this class
 * to define these dimensions so that we know exactly which part
 * of the tile map is within the view (on-screen); this allows us
 * to avoid rendering tiles that are off of the screen, so as to
 * avoid wasting computing power.
 *
 * \author Tyler Petresky <hello@tylerpetresky.com>
 * \date 03-22-16
 */
class MapFrame
{
public:
  //! X index of the top-left tile to draw
  int tile_x_start;
  //! Y index of the top-left tile to draw
  int tile_y_start;

  //! Number of pixels on the x-axis that we are from being positioned perfectly on a tile
  float tile_x_offset;
  //! Number of pixels on the y-axis that we are from being positioned perfectly on a tile
  float tile_y_offset;

  //! Number of tiles to draw along the x-axis
  unsigned int num_draw_x_axis;
  //! Number of tiles to draw along the y-axis
  unsigned int num_draw_y_axis;

  //! Coordinates of the screen edges
  MapRectangle screen_edges;
}; // class MapFrame

} // namespace private_map_mode

} // namespace rpg_map_mode

#endif // MAP_UTILS_HPP
