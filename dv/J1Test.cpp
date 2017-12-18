#include "dv/BinaryJitT.hpp"
#include "dv/J1.hpp"
#include "dv/JitBuilder.hpp"

#include "gtest/gtest.h"

TEST(J1, Add) {
  auto core = new Vj1("Core");
  Verilated::traceEverOn(true);
  auto tfp = new VerilatedVcdC;
  core->trace(tfp, 99);
  tfp->open("Add.vcd");

  dv::jit::JitBuilder<dv::jit::BinaryJitT> bld;
  bld = bld.noop()
            .imm(1)
            .imm(2)
            .imm(3)
            .add()
            .add()
            .add()
            // print out 'hello'
            .imm('h')
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
            .imm('\n')
            .imm(0)
            .to_io()
            // jump unconditionally to start
            .imm(0)
            .conditional_jump(0);

  std::vector<uint32_t> data_memory;

  for (int i = 0; i < 1000; i++) {
    data_memory.push_back(0);
    bld = bld.noop();
  }

  std::vector<uint16_t> instruction_memory = bld.jit_.ins;

  dv::sim::Memory m = {instruction_memory, data_memory};
  dv::sim::simulate(core, m, tfp);

  tfp->close();
}

TEST(J1, Add2) {
  auto core = new Vj1("Core");
  Verilated::traceEverOn(true);
  auto tfp = new VerilatedVcdC;
  core->trace(tfp, 99);
  tfp->open("Add2.vcd");

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
    bld = bld.noop();
  }

  std::vector<uint16_t> instruction_memory = bld.jit_.ins;

  dv::sim::Memory m = {instruction_memory, data_memory};
  dv::sim::simulate(core, m, tfp);

  tfp->close();
}

TEST(J1, Xor) {
  auto core = new Vj1("Core");
  Verilated::traceEverOn(true);
  auto tfp = new VerilatedVcdC;
  core->trace(tfp, 99);
  tfp->open("Add2.vcd");

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
  dv::sim::simulate(core, m, tfp);

  tfp->close();
}

TEST(J1, Jump) {
  auto core = new Vj1("Core");
  Verilated::traceEverOn(true);
  auto tfp = new VerilatedVcdC;
  core->trace(tfp, 99);
  tfp->open("Jump.vcd");

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
  dv::sim::simulate(core, m, tfp);

  tfp->close();
}
