#ifndef MAP_UTILS_HPP
#define MAP_UTILS_HPP

namespace rpg_map_mode
{

namespace private_map_mode
{

/**
 *
 */
class MapRectangle
{
public:
  /**
   * Default constructor
   */
  MapRectangle()
    : left(0.f)
    , right(0.f)
    , top(0.f)
    , bottom(0.f)
  {}

  /**
   * Constructor
   */
  MapRectangle(const float _left, const float _top,
               const float _right, const float _bottom)
    : left(_left)
    , right(_right)
    , top(_top)
    , bottom(_bottom)
  {}

  // Define the edges of the rectangle
  float left;
  float top;
  float right;
  float bottom;

  /**
   * Static method to check for intersection between two objects
   * of the type, MapRectangle. This is used extensively for
   * collision detection.
   * @param  _one [description]
   * @param  _two [description]
   * @return      [description]
   */
  static bool CheckIntersection(const MapRectangle& _one, const MapRectangle& _two);
};



/**
 *
 */
class MapFrame
{
public:
  // The x and y indices of the top-left tile to draw
  unsigned int tile_x_start, tile_y_start;

  // The x and y position screen coordinates to start drawing from
  float tile_x_offset, tile_y_offset;

  // Numer of tiles to draw on each axis
  unsigned int num_draw_x_axis, num_draw_y_axis;

  /** \brief The position coordinates of the screen edges.
  *** These members are in terms of the map grid 16x16 pixel coordinates that map objects use.
  *** The presense of these coordinates make it easier for map objects to figure out whether or
  *** not they should be drawn on the screen. Note that these are <b>not</b> used as drawing
  *** cursor positions, but rather are map grid coordinates indicating where the screen edges lie.
  **/
  MapRectangle screen_edges;
}; // class MapFrame

}

}

#endif // MAP_UTILS_HPP
