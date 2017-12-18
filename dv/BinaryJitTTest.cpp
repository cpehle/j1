#include "dv/BinaryJitT.hpp"
#include "dv/JitBuilder.hpp"

#include "gtest/gtest.h"

#include <iostream>

TEST(JIT, BinaryJitT) {
  dv::jit::JitBuilder<dv::jit::BinaryJitT> bld;

  bld = bld.imm(12).imm(2).add();

  for (auto i : bld.jit_.ins) {
    std::cout << i << std::endl;
  }
}
