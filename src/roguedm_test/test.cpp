// Copyright 2018 Fco. Jose Fdez. Naranjo AKA github.com/fjfnaranjo

#include "gtest/gtest.h"

#include "../roguedm/network/Network.hpp"

TEST(testMath, myCubeTest) {
  auto initial = new roguedm::Network();
  EXPECT_EQ(1000, 1000);
  delete initial;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
