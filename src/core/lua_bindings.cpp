#include "../utils/globals.hpp"

#include "script.hpp"
#include "../modes/map/map_mode.hpp"
#include "../modes/map/map_sprites.hpp"

#include "../utils/lua_bridge.hpp"
  using namespace luabridge;

using namespace rpg_script;

void BindEngineCode()
{
  PRINT_DEBUG << "Binding C++ function to Lua" << endl;

  using namespace rpg_map_mode;
  using namespace private_map_mode;
  getGlobalNamespace(ScriptManager->GetGlobalState())
    .beginNamespace("rpg_map_mode")
      .beginClass<MapMode>("MapMode")
        .addStaticFunction("CurrentInstance", &MapMode::CurrentInstance)
        .addFunction("SetCamera", &MapMode::SetCamera)
      .endClass()

      .beginClass<MapObject>("MapObject")
        .addFunction("SetPosition", &MapObject::SetPosition)
        .addFunction("SetDimensions", &MapObject::SetDimensions)
      .endClass()
      .deriveClass<VirtualSprite, MapObject>("VirtualSprite")
      .endClass()
      .deriveClass<MapSprite, VirtualSprite>("MapSprite")
        .addStaticFunction("Create", &MapSprite::Create)
        .addFunction("LoadAnimations", &MapSprite::LoadAnimations)
      .endClass()
      .deriveClass<EnemySprite, MapSprite>("EnemySprite")
        .addStaticFunction("Create", &EnemySprite::Create)
      .endClass();
}
