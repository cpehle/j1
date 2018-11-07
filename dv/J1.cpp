#include "obj_dir/Vj1.h"

#include "verilated.h"
#include "verilated_vcd_c.h"

#include "dv/J1.hpp"

#include <iostream>
#include <vector>

namespace dv {
namespace sim {

void simulate(std::unique_ptr<Vj1> core, Memory &m, VerilatedVcdC *tfp) {
  core->clk = 0;
  core->insn = 0;
  core->mem_din = 0;

  int t = 0;
  for (int i = 0; i < 100; i++) {

    core->io_din = 0;
    uint32_t iaddr = core->code_addr;
    uint32_t daddr = core->mem_addr;
    if (i < 5) {
      core->resetq = 0;
    } else {
      core->resetq = 1;
    }
    core->clk = 1;
    core->eval();
    t += 10;
    tfp->dump(t);

    if (iaddr >= m.instruction_memory.size()) {
      std::cout << "fetch out of bound instruction" << std::endl;
      core->insn = 0x6000; // noop
    } else {
      core->insn = m.instruction_memory[iaddr];
    }

    if (daddr / 4 >= m.data_memory.size()) {
      std::cout << "fetch out of bound data" << std::endl;
      core->mem_din = 0;
    } else {
      core->mem_din = m.data_memory[daddr / 4];
    }

    core->clk = 0;
    core->eval();
    t += 10;
    if (core->io_wr) {
      if (core->dout == '#') {
        break;
      }
      std::cout << static_cast<char>(core->dout);
    }
    tfp->dump(t);
  }
  core->final();
}

void step(std::unique_ptr<Vj1> core, Memory &m) {
  core->clk = 0;
  core->insn = 0;
  core->mem_din = 0;
  core->io_din = 0;  
  core->eval();

  // 
  uint32_t iaddr = core->code_addr;
  uint32_t daddr = core->mem_addr;

  if (iaddr >= m.instruction_memory.size()) {
    std::cout << "fetch out of bound instruction" << std::endl;
    core->insn = 0x6000; // noop
  } else {
    core->insn = m.instruction_memory[iaddr];
  }

  if (daddr / 4 >= m.data_memory.size()) {
    std::cout << "fetch out of bound data" << std::endl;
    core->mem_din = 0;
  } else {
    core->mem_din = m.data_memory[daddr / 4];
  }
  core->eval();
  if (core->io_wr) {
    if (core->dout == '#') {
      return;
    }
    std::cout << static_cast<char>(core->dout);
  }
}
}
}
