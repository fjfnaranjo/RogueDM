#include "gtest/gtest.h"

#include "../roguedm/InitialState.hpp"

TEST(testMath, myCubeTest) {
  auto initial = new roguedm::InitialState();
  EXPECT_EQ(1000, 1000);
  delete initial;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
