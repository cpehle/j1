#include "dv/BinaryJitT.hpp"
#include "dv/J1.hpp"
#include "dv/JitBuilder.hpp"

#include "gtest/gtest.h"

TEST(J1, HelloWorld) {
  Verilated::traceEverOn(true);
  auto core = std::make_unique<Vj1>("Core");
  auto tfp = new VerilatedVcdC;
  dv::jit::JitBuilder<dv::jit::BinaryJitT> bld;

  // print out hello
  bld = bld.imm('h')
            .imm(0)
            .to_io()
            .imm('e')
            .imm(0)
            .to_io()
            .imm('l')
            .imm(0)
            .to_io()
            .imm('l')
            .imm(0)
            .to_io()
            .imm('o')
            .imm(0)
            .to_io()
            .imm(' ')
            .imm(0)
            .to_io()
            .imm('w')
            .imm(0)
            .to_io()
            .imm('o')
            .imm(0)
            .to_io()
            .imm('r')
            .imm(0)
            .to_io()
            .imm('l')
            .imm(0)
            .to_io()
            .imm('d')
            .imm(0)
            .to_io()
            .imm('\n')
            .imm(0)
            .to_io()
            .imm('#')
            .imm(0)
            .to_io();

  std::vector<uint32_t> data_memory;

  for (int i = 0; i < 1000; i++) {
    data_memory.push_back(0);
    bld = bld.noop();
  }

  std::vector<uint16_t> instruction_memory = bld.jit_.ins;

  dv::sim::Memory m = {instruction_memory, data_memory};
  dv::sim::simulate(std::move(core), m, tfp);

  tfp->close();
}

TEST(J1, Add2) {
  auto core = std::make_unique<Vj1>("Core");
  Verilated::traceEverOn(true);
  auto tfp = new VerilatedVcdC;

  dv::jit::JitBuilder<dv::jit::BinaryJitT> bld;
  bld = bld.noop()
            .noop()
            .imm(1)
            .imm(2)
            .add()
            .imm(3)
            .equal()
            .conditional_jump(0)
            .imm('t')
            .imm(0)
            .to_io()
            .imm('#')
            .imm(0)
            .to_io();

  std::vector<uint32_t> data_memory;

  for (int i = 0; i < 1000; i++) {
    data_memory.push_back(0);
  }

  std::vector<uint16_t> instruction_memory{bld.jit_.ins};
  dv::sim::Memory m = {instruction_memory, data_memory};
  dv::sim::simulate(std::move(core), m, tfp);

  tfp->close();
}

TEST(J1, Xor) {
  auto core = std::make_unique<Vj1>("Core");
  Verilated::traceEverOn(true);
  auto tfp = new VerilatedVcdC;

  dv::jit::JitBuilder<dv::jit::BinaryJitT> bld;
  bld = bld.noop()
            .noop()
            .imm(0xf0f)
            .imm(0x0ff)
            .lxor()
            .imm(0xf0f ^ 0x0ff)
            .equal()
            .conditional_jump(0)
            .imm('t')
            .imm(0)
            .to_io()
            .imm('#')
            .imm(0)
            .to_io();

  std::vector<uint32_t> data_memory;

  for (int i = 0; i < 1000; i++) {
    data_memory.push_back(0);
    bld = bld.noop();
  }

  std::vector<uint16_t> instruction_memory = bld.jit_.ins;

  dv::sim::Memory m = {instruction_memory, data_memory};
  dv::sim::simulate(std::move(core), m, tfp);

  tfp->close();
}

TEST(J1, Jump) {
  auto core = std::make_unique<Vj1>("Core");
  auto tfp = new VerilatedVcdC;

  dv::jit::Label start;

  dv::jit::JitBuilder<dv::jit::BinaryJitT> bld;
  bld = bld.noop()
    .noop()
    .label(start)
    .imm('t')
    .imm(0)
    .to_io()
    .jump(start)
    .imm('#')
    .imm(0)
    .to_io();

  std::vector<uint32_t> data_memory;

  for (int i = 0; i < 1000; i++) {
    data_memory.push_back(0);
    bld = bld.noop();
  }

  std::vector<uint16_t> instruction_memory = bld.jit_.ins;

  dv::sim::Memory m = {instruction_memory, data_memory};
  dv::sim::simulate(std::move(core), m, tfp);

  tfp->close();
}
