#include "../utils/globals.hpp"

#include "script.hpp"
#include "../modes/map/map_mode.hpp"
#include "../modes/map/map_sprites.hpp"
#include "../modes/map/map_events.hpp"
#include "../modes/map/map_zones.hpp"

#include "global/global.hpp"

#include "../utils/sol2.hpp"

using namespace rpg_script;

void BindEngineCode()
{
  PRINT_DEBUG << "Binding C++ functions to Lua" << endl;

  using namespace rpg_map_mode;
  using namespace private_map_mode;
  using namespace rpg_global;

  sol::table map_mode_namespace = ScriptManager->GetGlobalState()->create_named_table("rpg_map_mode");
  map_mode_namespace.new_usertype<MapMode>("MapMode",
    "CurrentInstance", &MapMode::CurrentInstance,
    "GetEventSupervisor", &MapMode::GetEventSupervisor,
    "SetCamera", &MapMode::SetCamera
  );
  map_mode_namespace.new_usertype<MapObject>("MapObject",
    "SetPosition", &MapObject::SetPosition,
    "SetDimensions", &MapObject::SetDimensions,
    "GetXPosition", &MapObject::GetXPosition,
    "GetYPosition", &MapObject::GetYPosition
  );
  map_mode_namespace.new_usertype<VirtualSprite>("VirtualSprite",
    "SetDirection", &VirtualSprite::SetDirectionInt,
    sol::base_classes, sol::bases<MapObject>()
  );
  map_mode_namespace.new_usertype<MapSprite>("MapSprite",
    "Create", &MapSprite::Create,
    "LoadAnimations", &MapSprite::LoadAnimations,
    sol::base_classes, sol::bases<MapObject, VirtualSprite>()
  );
  map_mode_namespace.new_usertype<EnemySprite>("EnemySprite",
    "Create", &EnemySprite::Create,
    sol::base_classes, sol::bases<MapSprite, MapObject, VirtualSprite>()
  );

  map_mode_namespace.new_usertype<EventSupervisor>("EventSupervisor",
    "LaunchEventById", &EventSupervisor::LaunchEventById
  );
  map_mode_namespace.new_usertype<MapEvent>("MapEvent",
    "IsFinished", &private_map_mode::MapEvent::IsFinished,
    "GetEventId", &private_map_mode::MapEvent::GetEventId
  );
  map_mode_namespace.new_usertype<MapTransitionEvent>("MapTransitionEvent",
    "Create", &MapTransitionEvent::Create,
    sol::base_classes, sol::bases<MapEvent>()
  );

  map_mode_namespace.new_usertype<MapZone>("MapZone");
  map_mode_namespace.new_usertype<CameraZone>("CameraZone",
    "Create", &CameraZone::Create,
    "IsCameraInside", &CameraZone::IsCameraInside,
    sol::base_classes, sol::bases<MapZone>()
  );


  sol::table global_namespace = ScriptManager->GetGlobalState()->create_named_table("rpg_global");
  global_namespace.new_usertype<GlobalEngine>("GlobalEngine",
    "GetPreviousLocation", &GlobalEngine::GetPreviousLocation,
    "SetPreviousPosition", &GlobalEngine::SetPreviousPosition,
    "GetPreviousX", &GlobalEngine::GetPreviousX,
    "GetPreviousY", &GlobalEngine::GetPreviousY
  );
  global_namespace["GlobalManager"] = rpg_global::GlobalManager;
}
