#include "utils/globals.hpp"

#include "core/global/global.hpp"
#include "core/video_manager.hpp"
#include "core/mode_manager.hpp"
#include "core/resources.hpp"
#include "core/script.hpp"
#include "core/system.hpp"
#include "core/input.hpp"

#include "modes/map/map_mode.hpp"

using namespace rpg_global;
using namespace rpg_system;
using namespace rpg_mode_manager;
using namespace rpg_video;
using namespace rpg_resource;
using namespace rpg_script;
using namespace rpg_input;

bool InitializeSingletons();
void DeconstructSingletons();

int main()
{
  atexit(DeconstructSingletons);
  //sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(1024, 720), "Test");

  if (!InitializeSingletons())
  {
    PRINT_ERROR << "Failed to initialize singletons" << endl;
    DeconstructSingletons();
    exit(100);
  }

  GlobalManager->NewGame();

  ModeManager->Push(new rpg_map_mode::MapMode("data/maps/test.lua"));

  while (!SystemManager->IsExiting())
  {
    InputManager->CaptureEvents();
    InputManager->CaptureInput();


    VideoManager->Clear();

    ModeManager->Draw();

    VideoManager->DrawDebugInfo();
    VideoManager->Display();

    SystemManager->UpdateTimers();
    ModeManager->Update();
    VideoManager->Update();
  }

  VideoManager->Close();

  return 0;
}

bool InitializeSingletons()
{
  ModeManager = ModeEngine::CreateSingleton();
  SystemManager = SystemEngine::CreateSingleton();
  VideoManager = VideoEngine::CreateSingleton();
  GlobalManager = GlobalEngine::CreateSingleton();
  ResourceManager = ResourceEngine::CreateSingleton();
  ScriptManager = ScriptEngine::CreateSingleton();
  InputManager = InputEngine::CreateSingleton();

  if (!ModeManager->InitSingleton())
  {
    PRINT_ERROR << "Failed to initialize ModeEngine singleton" << endl;
    return false;
  }

  if (!SystemManager->InitSingleton())
  {
    PRINT_ERROR << "Failed to intialize SystemEngine singleton" << endl;
    return false;
  }

  if (!VideoManager->InitSingleton())
  {
    PRINT_ERROR << "Failed to initialize VideoEngine singleton" << endl;
    return false;
  }
  VideoManager->CreateWindow(1024, 768, "Test Window");

  if (!GlobalManager->InitSingleton())
  {
    PRINT_ERROR << "Failed to initialize GlobalEngine singleton" << endl;
    return false;
  }

  if (!ResourceManager->InitSingleton())
  {
    PRINT_ERROR << "Failed to initialize ResourceEngine singleton" << endl;
    return false;
  }

  if (!ScriptManager->InitSingleton())
  {
    PRINT_ERROR << "Failed to initialize ScriptEngine singleton" << endl;
    return false;
  }

  if (!InputManager->InitSingleton())
  {
    PRINT_ERROR << "Failed to initialize InputEngine singleton" << endl;
    return false;
  }

  return true;
}

void DeconstructSingletons()
{
  ModeEngine::DestroySingleton();
  GlobalEngine::DestroySingleton();
  SystemEngine::DestroySingleton();
  VideoEngine::DestroySingleton();
  ResourceEngine::DestroySingleton();
  ScriptEngine::DestroySingleton();
  InputEngine::DestroySingleton();
}
