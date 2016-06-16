/**
 * \file map_utils.hpp
 *
 * This file houses any smaller, extraneous classes that
 * are useful to the Map Mode. This includes things like:
 *  - MapRectangle (useful for collision detection)
 *  - MapFrame (defines the parameters associated with the camera)
 */

#ifndef MAP_UTILS_HPP
#define MAP_UTILS_HPP

namespace rpg_map_mode
{

namespace private_map_mode
{

  /** \name Screen Coordiante System Constants
  *** \brief Represents the size of the visible screen in map tiles and the collision grid
  *** Every map tile is 32x32 pixels, and every collision grid element is one quarter of that
  *** area (16x16). Thus the number of collision grid elements that compose the screen are
  *** four times the number of tiles that are visible on the screen. This also means the number
  *** of rows and columns of grid elements that encompass the screen are twice that of the
  *** number of rows and columns of tiles.
  **/
  //@{


  const float SCREEN_GRID_X_LENGTH = 64.0f; // was 32 for zoom ratio 2.0f;
  const float SCREEN_GRID_Y_LENGTH = 48.0f;
  const float HALF_SCREEN_GRID_X_LENGTH = SCREEN_GRID_X_LENGTH / 2;
  const float HALF_SCREEN_GRID_Y_LENGTH = SCREEN_GRID_Y_LENGTH / 2;

  const int TILES_ON_X_AXIS = (int)SCREEN_GRID_X_LENGTH / 2; // Number of tile columns that fit on the screen
  const int TILES_ON_Y_AXIS = (int)SCREEN_GRID_Y_LENGTH / 2; // Number of tile rows that fit on the screen
  const int HALF_TILES_ON_X_AXIS = TILES_ON_X_AXIS / 2;
  const int HALF_TILES_ON_Y_AXIS = TILES_ON_Y_AXIS / 2;

  // Length of a grid element in pixels (taken from the grid to screen width ratio)
  //const uint16_t GRID_LENGTH = vt_video::VIDEO_STANDARD_RES_WIDTH / SCREEN_GRID_X_LENGTH;
  // Length of a tile in pixels
  //const uint16_t TILE_LENGTH = GRID_LENGTH * 2;
  //@}

  enum CollisionType {
      NO_COLLISION = 0,
      CHARACTER_COLLISION = 1,
      ENEMY_COLLISION = 2,
      WALL_COLLISION = 3
  };
  const int ALL_COLLISION = CHARACTER_COLLISION | ENEMY_COLLISION | WALL_COLLISION;

  enum ObjectType {
    OBJECT_TYPE   = -1, //! Default type
    PHYSICAL_TYPE = 0,  //! This is a physical (visible) object, like a rock, a door, ect...
    VIRTUAL_TYPE  = 1,  //! This is used as another type of NPC sprite, but without interaction.
    SPRITE_TYPE   = 2,  //! This is a NPC sprite. Blocking for the hero, but not for the other sprites.
    ENEMY_TYPE    = 3  //! This is an enemy sprite. Blocking the hero, and triggering a battle.
    // TREASURE_TYPE = 4,  /** This is a treasure, can obtain a treasure from it when exploring,
    //                     but blocking for movement. */
    // SAVE_TYPE     = 5,  //! This is a save point. The player can save while its character is in it.
    // TRIGGER_TYPE  = 6,  //! A trigger object.
    //
    // //! Objects without possible interaction
    // //! Those objects are ignored when searching for the nearest interactable object.
    // HALO_TYPE     = 7,  //! This is a source of light, here for eye candy.
    // LIGHT_TYPE    = 8,  //! Another light type, but will change dynamically according to the map viewpoint.
    // PARTICLE_TYPE = 9,  //! A particle object.
    // SOUND_TYPE    = 10, //! An environmental sound
    // SCENERY_TYPE  = 11  //! The object is a scenery animal or harmless thing, and nothing should collide with.
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
  MapRectangle(const int _left, const int _top,
               const int _right, const int _bottom)
    : left(_left)
    , top(_top)
    , right(_right)
    , bottom(_bottom)
  {}

  //! X-value of the left of the rectangle
  int left;
  //! Y-value of the top of the rectangle
  int top;
  //! X-value of the right of the rectangle
  int right;
  //! Y-value of the bottom of the rectangle
  int bottom;

  /**
   * Static method to check for intersection between two objects
   * of the type, MapRectangle. This is used extensively for
   * collision detection.
   *
   * \note Parameter order does not matter.
   *
   * \param  _one First rectangle to compare
   * \param  _two Second rectangle to compare
   * \return      True if _one and _two are intersecting
   * 							somewhere, false otherwise
   */
  static bool CheckIntersection(const MapRectangle& _one, const MapRectangle& _two);
};



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
