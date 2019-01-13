#include "gtest/gtest.h"

#include "../roguedm/IORemote.hpp"

TEST(testMath, myCubeTest) {
  auto initial = new roguedm::IORemote();
  EXPECT_EQ(1000, 1000);
  delete initial;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
