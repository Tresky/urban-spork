#include <UnitTest++/UnitTest++.h>
#include "../../src/utils/globals.hpp"
#include "../../src/core/video_manager.hpp"

using namespace rpg_video;

SUITE(VideoEngineTest)
{
  class VideoEngineFixture
  {
  public:
    VideoEngineFixture()
    {
      VideoManager = VideoEngine::CreateSingleton();
      VideoManager->InitSingleton();
    }

    ~VideoEngineFixture()
    {
      VideoEngine::DestroySingleton();
    }
  };

  TEST_FIXTURE(VideoEngineFixture, CreateWindow)
  {
    VideoManager->CreateWindow(100, 110, "Test Title");
    CHECK_EQUAL(VideoManager->GetScreenWidth(), 100);
    CHECK_EQUAL(VideoManager->GetScreenHeight(), 110);
  }
}
