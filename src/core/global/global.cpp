#include "../../utils/globals.hpp"
#include "global.hpp"

#include "global_actors.hpp"
#include "../input.hpp"
#include "../video_manager.hpp"

namespace rpg_global
{

GlobalEngine* GlobalManager = nullptr;
bool GLOBAL_DEBUG = true;

GlobalEngine::GlobalEngine()
  : prev_loc("")
{
  IF_PRINT_DEBUG(GLOBAL_DEBUG) << "GlobalEngine constructor called" << endl;
}

GlobalEngine::~GlobalEngine()
{
  IF_PRINT_DEBUG(GLOBAL_DEBUG) << "GlobalEngine destructor called" << endl;
}

bool GlobalEngine::InitSingleton()
{
  return true;
}

GlobalCharacter* GlobalEngine::GetCharacter(const unsigned int _id)
{
  return global_characters[_id] ?
            global_characters[_id] : nullptr;
}

GlobalCharacter* GlobalEngine::GetHero()
{
  return global_characters[hero_id] ?
            global_characters[hero_id] : nullptr;
}

void GlobalEngine::NewGame()
{
  // global_characters[0] = new GlobalCharacter();
  // hero_id = 0;

  // if (!global_characters[0]->LoadAnimations("data/gchar/actor0-walking.lua"))
  // {
  //   PRINT_ERROR << "Failed to load animations for character" << endl;
  //   delete global_characters[0];
  //   global_characters[0] = nullptr;
  //   return;
  // }
  // global_characters[0]->SetCurrentAnimation("walk-south");
  // global_characters[0]->SetDirection("south");

  // global_characters[0]->SetPosition(rpg_video::VideoManager->GetScreenWidth() / 2.f - 16,
  //                                   rpg_video::VideoManager->GetScreenHeight() / 2.f - 16);
}

void GlobalEngine::Update()
{
  map<signed int, GlobalCharacter*>::iterator iter;
  for (iter = global_characters.begin(); iter != global_characters.end(); ++iter)
    iter->second->Update();
}

void GlobalEngine::DrawGlobalCharacters()
{
  map<signed int, GlobalCharacter*>::iterator iter;
  for (iter = global_characters.begin(); iter != global_characters.end(); ++iter)
    iter->second->Draw();
}

}
