#include "../../utils/globals.hpp"
#include "map_sprites.hpp"

#include "map_mode.hpp"
#include "map_stats.hpp"
#include "../../core/video_manager.hpp"
#include "../../core/script_read.hpp"
#include "../../core/system.hpp"
#include "../../core/image.hpp"

namespace rpg_map_mode
{

namespace private_map_mode
{

VirtualSprite::VirtualSprite(MapObjectDrawLayer _layer)
  : MapObject(_layer)
  , movement_speed(8.f)
  , moving(false)
  , has_moved(false)
  , attacking(false)
  , finished_attacking(true)
  , direction(DIRECTION_SOUTH)
{
  object_type = VIRTUAL_TYPE;
}

void VirtualSprite::Update()
{
  has_moved = false;

  if (IsMoving())
    SetNextPosition();
}

void VirtualSprite::SetDirection(const Direction _direction)
{
  direction = _direction;
}

void VirtualSprite::SetRandomDirection()
{
    switch(rand() % 8 + 1) {
    case 1:
        SetDirection(DIRECTION_NORTH);
        break;
    case 2:
        SetDirection(DIRECTION_NORTHWEST);
        break;
    case 3:
        SetDirection(DIRECTION_NORTHEAST);
        break;
    case 4:
        SetDirection(DIRECTION_SOUTH);
        break;
    case 5:
        SetDirection(DIRECTION_SOUTHWEST);
        break;
    case 6:
        SetDirection(DIRECTION_SOUTHEAST);
        break;
    case 7:
        SetDirection(DIRECTION_EAST);
        break;
    case 8:
        SetDirection(DIRECTION_WEST);
        break;
    default:
        IF_PRINT_WARNING(MAP_MODE_DEBUG) << "Invalid randomized direction was chosen" << endl;
    }
}

float VirtualSprite::CalculateDistanceMoved()
{
  float distance_moved = static_cast<float>(rpg_system::SystemManager->GetUpdateTime()) / movement_speed;

    // Double the distance to move if the sprite is running
    //if(_is_running == true)
        //distance_moved *= 2.0f;
    // If the movement is diagonal, decrease the lateral movement distance by sin(45 degress)
    //if(_direction & MOVING_DIAGONALLY)
      //  distance_moved *= 0.707f;

    // We cap the distance moved when in case of low FPS to avoid letting certain
    // sprites jump across blocking areas.
    // if (distance_moved > 1.0f)
    //   distance_moved = 1.0f;

    return distance_moved;
}

void VirtualSprite::SetNextPosition()
{
  // Next sprite's position holders
  float next_pos_x = static_cast<float>(GetPosition().x);
  float next_pos_y = static_cast<float>(GetPosition().y);

  // TPNOTE: This is an integer now because, for some reason, if
  // left as a float, characters move quicker up and left than they do
  // right and down. Still not sure why...
  // TPNOTE: This has to do with floating point precision. First step
  // to fixing this is to make all SetPosition() and GetPosition()
  // functions based on float's. However, this makes the screen jump on
  // offsets. Still not sure how to fix that.
  // TPTODO: This MUST be fixed or else we can't do diagonal movement properly.
  int distance_moved = CalculateDistanceMoved();

  // Move the sprite the appropriate distance in the appropriate Y and X direction
  if (direction == DIRECTION_NORTH || direction == DIRECTION_NORTHWEST || direction == DIRECTION_NORTHEAST)
    next_pos_y -= distance_moved;
  else if (direction == DIRECTION_SOUTH || direction == DIRECTION_SOUTHWEST || direction == DIRECTION_SOUTHEAST)
    next_pos_y += distance_moved;
  if (direction == DIRECTION_WEST || direction == DIRECTION_SOUTHWEST || direction == DIRECTION_NORTHWEST)
    next_pos_x -= distance_moved;
  else if (direction == DIRECTION_EAST || direction == DIRECTION_SOUTHEAST || direction == DIRECTION_NORTHEAST)
    next_pos_x += distance_moved;

  // When not moving, do not check anything else.
  if(next_pos_x == GetPosition().x && next_pos_y == GetPosition().y)
    return;

  // We've got the next position, let's check whether the next position
  // should be revised.

  // Used to know whether we could fall back to a straight move
  // in case of collision.
  bool moving_diagonally = (direction == DIRECTION_NORTHWEST || direction == DIRECTION_NORTHEAST ||
                            direction == DIRECTION_SOUTHEAST || direction == DIRECTION_SOUTHWEST);

  // Handle collision with the first object encountered
  MapObject* collision_object = nullptr;
  MapMode* map_mode = MapMode::CurrentInstance();
  ObjectSupervisor* object_supervisor = map_mode->GetObjectSupervisor();
  CollisionType collision_type = object_supervisor->DetectCollision(this, next_pos_x,
                    next_pos_y,
                    &collision_object);

  // Try to fall back to straight direction
  if(moving_diagonally && collision_type != NO_COLLISION) {
    // Try on x axis
    if(object_supervisor->DetectCollision(this, GetPosition().x, next_pos_y, &collision_object) == NO_COLLISION) {
      next_pos_x = GetPosition().x;
      collision_type = NO_COLLISION;
    } // and then on y axis
    else if(object_supervisor->DetectCollision(this, next_pos_x, GetPosition().y, &collision_object) == NO_COLLISION) {
      next_pos_y = GetPosition().y;
      collision_type = NO_COLLISION;
    }
  }

  // // Try to handle wall and physical collisions after a failed straight or diagonal move
  switch(collision_type) {
  case NO_COLLISION:
    default:
      break;
  case WALL_COLLISION:
    // When being blocked and moving diagonally, the npc is stuck.
    // if(moving_diagonally)
    //     return;

    // Don't consider physical objects with an event to avoid sliding on their edges,
    // making them harder to "talk with".
    // if (collision_object && this == map_mode->GetCamera()) {
    //     PhysicalObject *phs = reinterpret_cast<PhysicalObject *>(collision_object);
    //     if(phs && !phs->GetEventIdWhenTalking().empty())
    //         return;
    // }

    // Fix the direction and destination to walk-around obstacles
    // if (_HandleWallEdges(next_pos_x, next_pos_y, distance_moved, collision_object))
    //     break;
    // We don't do any other checks for the player sprite.
    if (this == map_mode->GetCamera())
      return;
    // NPC sprites:

    // When it's a true wall, try against the collision grid
    if (!collision_object) {
      // Try a random diagonal to avoid the wall in straight direction
      if (direction == DIRECTION_NORTH || direction == DIRECTION_SOUTH)
        direction = rand() % 1 ? DIRECTION_EAST : DIRECTION_WEST;
      else if(direction == DIRECTION_EAST || direction == DIRECTION_WEST)
        direction = rand() % 1 ? DIRECTION_NORTH : DIRECTION_SOUTH;
      return;
    }
        // // Physical and treasure objects are the only other matching "fake" walls
        // else {
        //     // Try a diagonal to avoid the sprite in straight direction by comparing
        //     // each one coords.
        //     float diff_x = GetXPosition() - collision_object->GetXPosition();
        //     float diff_y = GetYPosition() - collision_object->GetYPosition();
        //     if(_direction & (NORTH | SOUTH))
        //         _direction |= diff_x >= 0.0f ? EAST : WEST;
        //     else if(_direction & (EAST | WEST))
        //         _direction |= diff_y >= 0.0f ? SOUTH : NORTH;
        //     return;
        // }
        // Other cases shouldn't happen.
      break;
    case ENEMY_COLLISION:
      // Check only whether the player has collided with a monster
      if (this == map_mode->GetCamera() && collision_object &&
          collision_object->GetType() == ENEMY_TYPE) {
        // EnemySprite* enemy = reinterpret_cast<EnemySprite*>(collision_object);

        // Check whether the player is actually playing. If not, we don't want to start a battle.
        // if (map_mode->CurrentState() == STATE_EXPLORE)
        //     map_mode->StartEnemyEncounter(enemy);
        return;
      }

      break;
    case CHARACTER_COLLISION:
      // Check whether the sprite is tangled with another character, even without moving
      // For instance, when colliding with a path follower npc.
      // And let it through in that case.
      if (object_supervisor->CheckObjectCollision(GetGridCollisionRectangle(), collision_object)) {
        collision_type = NO_COLLISION;
        break;
      }

      // When the sprite is controlled by the camera, let the player handle the position correction.
      if (this == map_mode->GetCamera())
        return;

      // Check whether an enemy has collided with the player
      if (this->GetType() == ENEMY_TYPE &&
          collision_object == map_mode->GetCamera()) {
          // EnemySprite* enemy = reinterpret_cast<EnemySprite *>(this);
          //
          // // Check whether the player is actually playing. If not, we don't want to start a battle.
          // if (map_mode->CurrentState() == STATE_EXPLORE)
          //     map_mode->StartEnemyEncounter(enemy, false, true); // The enemy gets a boost in agility.
        return;
      }
    //
    //     // When being blocked and moving diagonally, the npc is stuck.
    //     if(moving_diagonally)
    //         return;
    //
    //     if(!collision_object)  // Should never happen
    //         return;
    //
    //     // Try a diagonal to avoid the sprite in straight direction by comparing
    //     // each one coords.
    //     float diff_x = GetXPosition() - collision_object->GetXPosition();
    //     float diff_y = GetYPosition() - collision_object->GetYPosition();
    //     if(_direction & (NORTH | SOUTH))
    //         _direction |= diff_x >= 0.0f ? EAST : WEST;
    //     else if(_direction & (EAST | WEST))
    //         _direction |= diff_y >= 0.0f ? SOUTH : NORTH;
    //     return;
    // }
    //
    // // Inform the overlay system of the parallax movement done if needed
    // if(this == map_mode->GetCamera()) {
    //     float x_parallax = !map_mode->IsCameraXAxisInMapCorner() ?
    //                        (GetXPosition() - next_pos_x) / SCREEN_GRID_X_LENGTH * VIDEO_STANDARD_RES_WIDTH :
    //                        0.0f;
    //     float y_parallax = !map_mode->IsCameraYAxisInMapCorner() ?
    //                        (GetYPosition() - next_pos_y) / SCREEN_GRID_Y_LENGTH * VIDEO_STANDARD_RES_HEIGHT :
    //                        0.0f;
    //
    //     map_mode->GetEffectSupervisor().AddParallax(x_parallax, y_parallax);
    //     map_mode->GetIndicatorSupervisor().AddParallax(x_parallax, y_parallax);
    }

    // Make the sprite advance at the end
    SetPosition(next_pos_x, next_pos_y);
    has_moved = true;
}

MapSprite::MapSprite(MapObjectDrawLayer _layer)
  : VirtualSprite(_layer)
{
  object_type = SPRITE_TYPE;
}

MapSprite::~MapSprite()
{
  // DELETE ANIMATIONS LATER
}

MapSprite* MapSprite::Create(const int _layer)
{
    return new MapSprite(static_cast<MapObjectDrawLayer>(_layer));
}

bool MapSprite::LoadAnimations(const std::string& _filepath)
{
  rpg_script::ReadScript sprite_script;

  if (!sprite_script.OpenFile(_filepath))
  {
    if (sprite_script.HasError())
      sprite_script.PrintErrors();
    PRINT_ERROR << "Failed to open animations script: " << _filepath << std::endl;
    return false;
  }

  sprite_script.OpenTable("animations"); // Opens animations

  std::string filepath = sprite_script.ReadData<std::string>("filepath", "");
  int num_animations = sprite_script.ReadData<int>("num_animations", -1);

  // Check for any errors so far
  if (sprite_script.HasError())
  {
    sprite_script.PrintErrors();
    return false;
  }

  // Loop through all animation tables
  for (int i = 0; i < num_animations; ++i)
  {
    sprite_script.OpenTable<int>(i); // Opens animation[i]

    std::string animation_name = sprite_script.ReadData<std::string>("name", "");

    int tile_width = sprite_script.ReadData<int>("width", -1);
    int tile_height = sprite_script.ReadData<int>("width", -1);

    int num_loops = sprite_script.ReadData<int>("num_loops", -2);

    int num_frames = sprite_script.ReadData<int>("num_frames", -1);
    int frame_time = sprite_script.ReadData<int>("frame_time", -1);

    sprite_script.OpenTable<std::string>("frame_rects"); // Opens frame_rects

    // Check for any errors so far
    if (sprite_script.HasError())
    {
      sprite_script.PrintErrors();
      return false;
    }

    animations[animation_name] = new rpg_video::AnimatedImage(tile_width, tile_height);
    for (int j = 1; j < num_frames + 1; ++j)
    {
      sprite_script.OpenTable<int>(j); // Open {x, y}

      int x = sprite_script.ReadData<int>("x", -1);
      int y = sprite_script.ReadData<int>("y", -1);

      // Check for any errors so far
      if (sprite_script.HasError())
      {
        sprite_script.PrintErrors();
        delete animations[animation_name];
        animations[animation_name] = nullptr;
        return false;
      }

      sf::IntRect rect = sf::IntRect(x, y, tile_width, tile_height);
      int resource_id = rpg_resource::ResourceManager->LoadImageRect(filepath, rect);
      if (resource_id < 0)
      {
        // If the ResourceManager failed to load the resource, we need
        // to clean up a hanging pointer.
        PRINT_WARNING << "Failed to load resource from: " << filepath << std::endl;
        delete animations[animation_name];
        animations[animation_name] = nullptr;
        return false;
      }

      animations[animation_name]->AddFrame(resource_id, frame_time);
      sprite_script.CloseTable(); // Closes {x, y}
    }
    sprite_script.CloseTable(); // Closes frame_rects

    animations[animation_name]->SetNumberLoops(num_loops);

    sprite_script.CloseTable(); // Closes animation[i]
  }
  return true;
}

void MapSprite::Update()
{
  //bool was_moved = has_moved;

  if (IsAttacking() && animations[current_animation]->IsFinished())
  {
    animations[current_animation]->Reset();
    attacking = false;
    finished_attacking = true;
  }

  // Collision detection stuffs
  VirtualSprite::Update();

  std::string new_anim = "";
  if (IsMoving())
    new_anim += "walk-";
  else if (IsAttacking())
  {
      animations[current_animation]->SetFinished(false);
      new_anim += "attack-";
      finished_attacking = false;
  }
  else
    new_anim += "idle-";

  if (direction == DIRECTION_NORTH || direction == DIRECTION_NORTHWEST || direction == DIRECTION_NORTHEAST)
    new_anim += "north";
  else if (direction == DIRECTION_SOUTH || direction == DIRECTION_SOUTHWEST || direction == DIRECTION_SOUTHEAST)
    new_anim += "south";
  else if (direction == DIRECTION_WEST || direction == DIRECTION_SOUTHWEST || direction == DIRECTION_NORTHWEST)
    new_anim += "west";
  else if (direction == DIRECTION_EAST || direction == DIRECTION_SOUTHEAST || direction == DIRECTION_NORTHEAST)
    new_anim += "east";

  current_animation = new_anim;

  if (!current_animation.empty())
    animations[current_animation]->Update();
}

void MapSprite::Draw()
{
  if (!current_animation.empty() && ShouldDraw())
  {
    MapMode* map_mode = MapMode::CurrentInstance();

    float x = map_mode->GetScreenXCoordinate(GetPosition().x);
    float y = map_mode->GetScreenYCoordinate(GetPosition().y);

    animations[current_animation]->SetPosition(x, y);
    animations[current_animation]->Draw();

    if (rpg_video::VideoManager->IsDebug())
    {
      MapRectangle rect = GetGridCollisionRectangle(x, y);
      sf::RectangleShape r(sf::Vector2f(rect.right - rect.left,
                                        rect.bottom - rect.top));
      r.setOutlineColor(sf::Color::Red);
      r.setOutlineThickness(1.f);
      r.setFillColor(sf::Color::Transparent);
      r.setPosition(rect.left, rect.top);
      rpg_video::VideoManager->DrawShape(r);
    }
  }
}

EnemySprite::EnemySprite()
  : MapSprite(GROUND_OBJECT)
  , state(State::SPAWNING)
  , time_elapsed(0)
  , attack_range(45)
  , base_aggro(20)
  , aggro_range(base_aggro)
  , aggro_box(0, 0, 0, 0)
  , stats(nullptr)
{
  object_type = ENEMY_TYPE;
  moving = false;

  stats = new Statistics();
  stats->SetMaxHealth(100);
  stats->SetCurrentHealth(100);
}

EnemySprite::~EnemySprite()
{}

EnemySprite* EnemySprite::Create()
{
  return new EnemySprite();
}

void EnemySprite::Update()
{
  switch (state)
  {
    case State::SPAWNING:
      state = State::HOSTILE;
      break;
    case State::HOSTILE:
      UpdateHostile();
      break;
    case State::DYING:
      current_animation = "death-south";

      if (!current_animation.empty())
      {
        if (animations[current_animation]->IsFinished())
        {
          cout << "Enemy Dead" << endl;
          state = State::DEAD;
        }
        animations[current_animation]->Update();
      }
      break;
    case State::DEAD:
    default:
      break;
  }
}

void EnemySprite::Draw()
{
  if (MapObject::ShouldDraw() &&
      !IsDead() &&
      !current_animation.empty())
  {
    MapMode* map_mode = MapMode::CurrentInstance();

    float x = map_mode->GetScreenXCoordinate(GetPosition().x);
    float y = map_mode->GetScreenYCoordinate(GetPosition().y);

    //cout << "X: " << x << " :: Y: " << y << endl;
    //cout << "POS: " << GetPosition().x << " :: " << GetPosition().y << endl;

    animations[current_animation]->SetPosition(x, y);
    animations[current_animation]->Draw();

    if (rpg_video::VideoManager->IsDebug())
    {
      MapRectangle rect = GetGridCollisionRectangle(x, y);
      sf::RectangleShape r(sf::Vector2f(rect.right - rect.left,
                                        rect.bottom - rect.top));
      r.setOutlineColor(sf::Color::Red);
      r.setOutlineThickness(1.f);
      r.setFillColor(sf::Color::Transparent);
      r.setPosition(rect.left, rect.top);
      rpg_video::VideoManager->DrawShape(r);

      r = sf::RectangleShape(sf::Vector2f(aggro_box.right - aggro_box.left,
                                          aggro_box.bottom - aggro_box.top));
      r.setOutlineColor(sf::Color::Yellow);
      r.setOutlineThickness(1.f);
      r.setFillColor(sf::Color::Transparent);

      float x = map_mode->GetScreenXCoordinate(aggro_box.left);
      float y = map_mode->GetScreenYCoordinate(aggro_box.top);
      r.setPosition(x, y);
      rpg_video::VideoManager->DrawShape(r);
    }
  }
}

void EnemySprite::UpdateHostile()
{
  if (stats->GetCurrentHealth() <= 0)
  {
    cout << "Enemy Dying" << endl;
    state = State::DYING;
  }

  VirtualSprite* camera = MapMode::CurrentInstance()->GetCamera();
  sf::Vector2i camera_pos = camera->GetCenterPosition();

  sf::Vector2i delta_pos = sf::Vector2i(GetCenterPosition().x - camera_pos.x,
                                        GetCenterPosition().y - camera_pos.y);

  if (abs(delta_pos.x) > rpg_video::VideoManager->GetScreenWidth() * 1.5 ||
      abs(delta_pos.y) > rpg_video::VideoManager->GetScreenHeight() * 1.5)
    return;

  MapSprite::Update();

  aggro_box.left = GetPosition().x - aggro_range;
  aggro_box.top = GetPosition().y - aggro_range;
  aggro_box.right = GetPosition().x + dimensions.x + aggro_range;
  aggro_box.bottom = GetPosition().y + dimensions.y + aggro_range;

  bool player_in_aggro = false;
  if (MapRectangle::CheckIntersection(aggro_box,
                                      camera->GetGridCollisionRectangle()))
    player_in_aggro = true;

  if (player_in_aggro)
  {
    aggro_range = base_aggro + 8;

    // if (MapRectangle::InRange(this->GetGridCollisionRectangle(),
    //                           camera->GetGridCollisionRectangle(), 3))
    sf::Vector2i this_pos = GetCenterPosition();
    float dist = sqrt(pow(this_pos.x - camera_pos.x, 2) + pow(this_pos.y - camera_pos.y, 2));
    if (dist <= attack_range)
    {
      cout << "In Attack Range" << endl;
      moving = false;
    }
    else
    {
      // cout << "Searching..." << endl;

      int tolerance = 3;

      if (delta_pos.x < 0 - tolerance && delta_pos.y < 0 - tolerance)
        SetDirection(DIRECTION_SOUTHEAST);
      else if (delta_pos.x < 0 - tolerance && delta_pos.y > 0 + tolerance)
        SetDirection(DIRECTION_NORTHEAST);
      else if (delta_pos.x < 0 - tolerance)
        SetDirection(DIRECTION_EAST);

      else if (delta_pos.x > 0 + tolerance && delta_pos.y < 0 - tolerance)
        SetDirection(DIRECTION_SOUTHWEST);
      else if (delta_pos.x > 0 + tolerance && delta_pos.y > 0 + tolerance)
        SetDirection(DIRECTION_NORTHWEST);
      else if (delta_pos.x > 0 + tolerance)
        SetDirection(DIRECTION_WEST);

      else if (delta_pos.y < 0)
        SetDirection(DIRECTION_SOUTH);
      else
        SetDirection(DIRECTION_SOUTH);
      moving = true;
    }
    return;
  }
  else
    aggro_range = base_aggro;

  // Handle monsters with way points.
  if (!way_points.empty())
  {
    moving = true;

    if (GetCenterPosition().x == way_points[current_way_point].x &&
        GetCenterPosition().y == way_points[current_way_point].y)
    {
      if (++current_way_point >= way_points.size())
        current_way_point = 0;
    }
    else
    {
      float x_delta = GetCenterPosition().x - way_points[current_way_point].x;
      float y_delta = GetCenterPosition().y - way_points[current_way_point].y;

      if (x_delta < 0 && y_delta < 0)
        SetDirection(DIRECTION_SOUTHEAST);
      else if (x_delta < 0 && y_delta > 0)
        SetDirection(DIRECTION_NORTHEAST);
      else if (x_delta < 0)
        SetDirection(DIRECTION_EAST);

      else if (x_delta > 0 && y_delta < 0)
        SetDirection(DIRECTION_SOUTHWEST);
      else if (x_delta > 0 && y_delta > 0)
        SetDirection(DIRECTION_NORTHWEST);
      else if (x_delta > 0)
        SetDirection(DIRECTION_WEST);

      else if (y_delta < 0)
        SetDirection(DIRECTION_SOUTH);
      else
        SetDirection(DIRECTION_NORTH);
    }

  }

  time_elapsed += rpg_system::SystemManager->GetUpdateTime();

}

void EnemySprite::AddWayPoint(const int _x, const int _y)
{
  // TPTODO: Check whether way point is already added
  way_points.push_back(sf::Vector2i(_x, _y));
}


void EnemySprite::TakeDamage(const int _raw)
{
  cout << "Taking Damage: " << _raw << endl;
  stats->SetCurrentHealth(stats->GetCurrentHealth() - _raw);
  cout << "Remaining Health: " << stats->GetCurrentHealth() << endl;
}

}

}
