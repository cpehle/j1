#pragma once

#include "obj_dir/Vj1.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include <vector>

namespace dv {
namespace sim {

struct Memory {
  std::vector<uint16_t> instruction_memory;
  std::vector<uint32_t> data_memory;
};
void simulate(std::unique_ptr<Vj1> core, Memory &m, VerilatedVcdC *tfp);
}
}
