#include <UnitTest++/UnitTest++.h>

// #include "core/test_system_engine.hpp"
#include "core/test_video_engine.hpp"
#include "core/test_image.hpp"

TEST(Sanity)
{
  CHECK_EQUAL(1, 1);
}

int main(int argc, char** args)
{
  return UnitTest::RunAllTests();
}
