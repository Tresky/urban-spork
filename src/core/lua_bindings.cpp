#include "../utils/globals.hpp"

#include "script.hpp"
#include "../modes/map/map_mode.hpp"
#include "../modes/map/map_sprites.hpp"
#include "../modes/map/map_events.hpp"
#include "../modes/map/map_zones.hpp"

#include "global/global.hpp"

#include "../utils/lua_bridge.hpp"
  using namespace luabridge;

using namespace rpg_script;

void BindEngineCode()
{
  PRINT_DEBUG << "Binding C++ function to Lua" << endl;

  using namespace rpg_map_mode;
  using namespace private_map_mode;
  using namespace rpg_global;

  getGlobalNamespace(ScriptManager->GetGlobalState())
    .beginNamespace("rpg_map_mode")
      .beginClass<MapMode>("MapMode")
        .addStaticFunction("CurrentInstance", &MapMode::CurrentInstance)
        .addFunction("GetEventSupervisor", &MapMode::GetEventSupervisor)
        .addFunction("SetCamera", &MapMode::SetCamera)
      .endClass()

      .beginClass<MapObject>("MapObject")
        .addFunction("SetPosition", &MapObject::SetPosition)
        .addFunction("SetCenterPosition", &MapObject::SetCenterPosition)
        .addFunction("SetDimensions", &MapObject::SetDimensions)
        .addFunction("GetXPosition", &MapObject::GetXPosition)
        .addFunction("GetYPosition", &MapObject::GetYPosition)
      .endClass()
      .deriveClass<VirtualSprite, MapObject>("VirtualSprite")
        .addFunction("SetDirection", &VirtualSprite::SetDirectionInt)
        .addFunction("SetMovementSpeed", &VirtualSprite::SetMovementSpeed)
      .endClass()
      .deriveClass<MapSprite, VirtualSprite>("MapSprite")
        .addStaticFunction("Create", &MapSprite::Create)
        .addFunction("LoadAnimations", &MapSprite::LoadAnimations)
      .endClass()
      .deriveClass<EnemySprite, MapSprite>("EnemySprite")
        .addStaticFunction("Create", &EnemySprite::Create)
        .addFunction("AddWayPoint", &EnemySprite::AddWayPoint)
      .endClass()

      .beginClass<EventSupervisor>("EventSupervisor")
        .addFunction("LaunchEventById", &EventSupervisor::LaunchEventById)
      .endClass()
      .beginClass<MapEvent>("MapEvent")
        .addFunction("IsFinished", &private_map_mode::MapEvent::IsFinished)
        .addFunction("GetEventId", &private_map_mode::MapEvent::GetEventId)
      .endClass()
      .deriveClass<MapTransitionEvent, MapEvent>("MapTransitionEvent")
        .addStaticFunction("Create", &MapTransitionEvent::Create)
      .endClass()

      .beginClass<MapZone>("MapZone")
      .endClass()
      .deriveClass<CameraZone, MapZone>("CameraZone")
        .addStaticFunction("Create", &CameraZone::Create)
        .addFunction("IsCameraInside", &CameraZone::IsCameraInside)
      .endClass()
    .endNamespace()

    .beginNamespace("rpg_global")
      .addVariable("GlobalManager", &rpg_global::GlobalManager)
      .beginClass<GlobalEngine>("GlobalEngine")
        .addFunction("GetPreviousLocation", &GlobalEngine::GetPreviousLocation)
        .addFunction("SetPreviousPosition", &GlobalEngine::SetPreviousPosition)
        .addFunction("GetPreviousX", &GlobalEngine::GetPreviousX)
        .addFunction("GetPreviousY", &GlobalEngine::GetPreviousY)
      .endClass();
}
