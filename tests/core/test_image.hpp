#include <UnitTest++/UnitTest++.h>
#include "../../src/utils/globals.hpp"
#include "../../src/core/image.hpp"

using namespace rpg_video;

bool VIDEO_DEBUG = false;

SUITE(ImageTest)
{
  class ImageFixture
  {
  public:
    Image image1;
    Image image2;
  };

  // TPTODO: Generate test images (squares of color, for testing for the white square of nullptr)
  TEST_FIXTURE(ImageFixture, LoadResource)
  {
    image1.LoadResource("../data/entities/actor0.png");
    CHECK_EQUAL(384, image1.GetWidth());
    CHECK_EQUAL(256, image1.GetHeight());
  }

  TEST_FIXTURE(ImageFixture, LoadResourceFromImage)
  {
    sf::Image temp;
    temp.loadFromFile("../data/entities/actor0.png");

    sf::IntRect rect = sf::IntRect(0, 0, 32, 64);
    image1.LoadResourceFromImage(temp, rect);
    CHECK_EQUAL(32, image1.GetWidth());
    CHECK_EQUAL(64, image1.GetHeight());
  }

  TEST_FIXTURE(ImageFixture, CopyConstructor)
  {
    image1.LoadResource("../data/entities/actor0.png");
    Image temp(image1);
    CHECK_EQUAL(384, temp.GetWidth());
    CHECK_EQUAL(256, temp.GetHeight());
  }

  TEST_FIXTURE(ImageFixture, AssignmentOperator)
  {
    image1.LoadResource("../data/entities/actor0.png");
    image2 = image1;
    CHECK_EQUAL(384, image2.GetWidth());
    CHECK_EQUAL(256, image2.GetHeight());
  }

  TEST_FIXTURE(ImageFixture, SetPositionPositive)
  {
    image1.LoadResource("../data/entities/actor0.png");
    image1.SetPosition(32, 32);
    CHECK_EQUAL(32, image1.GetPosition().x);
    CHECK_EQUAL(32, image1.GetPosition().y);
  }

  TEST_FIXTURE(ImageFixture, SetPositionNegative)
  {
    image1.LoadResource("../data/entities/actor0.png");
    image1.SetPosition(-32, -32);
    CHECK_EQUAL(-32, image1.GetPosition().x);
    CHECK_EQUAL(-32, image1.GetPosition().y);
  }
}
