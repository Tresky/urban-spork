#include "script_read.hpp"

#include "../utils/lua_bridge.hpp"
using namespace luabridge;

namespace rpg_script
{

ReadScript::ReadScript()
  : L(nullptr)
{}

ReadScript::~ReadScript()
{
  CloseFile();
}

bool ReadScript::OpenFile(const std::string& _filename)
{
  // These return 0 on success, so the logic is backwards
  if (luaL_loadfile(L, _filename.c_str()) || lua_pcall(L, 0, 0, 0))
  {
    PRINT_ERROR << "Script failed to load: " << _filename << endl;
    return false;
  }

  filename = _filename;
  return true;
}

void ReadScript::CloseFile()
{
  if (L)
    lua_close(L);
}
