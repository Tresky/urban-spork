#include "../../utils/globals.hpp"
#include "global.hpp"

#include "global_actors.hpp"
#include "../input.hpp"

namespace rpg_global
{

GlobalEngine* GlobalManager = nullptr;
bool GLOBAL_DEBUG = true;

GlobalEngine::GlobalEngine()
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
  global_characters[0] = new GlobalCharacter();
  hero_id = 0;

  int id = rpg_resource::ResourceManager->LoadImage("data/gchar/hero.png");
  if (id < 0)
  {
    PRINT_ERROR << "Failed to load character" << endl;
    delete global_characters[0];
    global_characters[0] = nullptr;
    return;
  }

  global_characters[0]->SetMapImage(id);
  global_characters[0]->SetPosition(32, 32);

  // Let the input manager about the hero
  //rpg_input::InputManager->CheckGlobalForHero();

  // This will change later
  //global_character[0] = new GlobalCharacter("path/to/sprite");
}

void GlobalEngine::DrawGlobalCharacters()
{
  map<signed int, GlobalCharacter*>::iterator iter;
  for (iter = global_characters.begin(); iter != global_characters.end(); ++iter)
    iter->second->Draw();
}

}
