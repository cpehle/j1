#pragma once
#include "dv/JitBuilder.hpp"
#include <vector>

namespace dv {
namespace jit {
namespace {

const uint16_t T = 0x0000;
const uint16_t N = 0x0100;
const uint16_t T_plus_N = 0x0200;
const uint16_t T_and_N = 0x0300;
const uint16_t T_or_N = 0x0400;
const uint16_t T_xor_N = 0x0500;
const uint16_t invert_T = 0x0600;
const uint16_t N_equal_T = 0x0700;
const uint16_t N_lt_T = 0x0800;
const uint16_t N_shiftr_T = 0x0900;
const uint16_t N_shiftl_T = 0x0a00;
const uint16_t rT = 0x0b00;
const uint16_t read_mem_T = 0x0c00;
const uint16_t read_io_T = 0x0d00;
const uint16_t status = 0x0e00;
const uint16_t N_ult_T = 0x0f00;

// targets
const uint16_t T_to_N = 0x0010;
const uint16_t T_to_R = 0x0020;
const uint16_t N_to_memT = 0x0030;
const uint16_t N_to_ioT = 0x0040;
const uint16_t RET = 0x0080;

// stack operators
const uint16_t d_minus_1 = 0x0003;
const uint16_t d_plus_1 = 0x0001;
const uint16_t r_minus_1 = 0x000c;
const uint16_t r_minus_2 = 0x0008;
const uint16_t r_plus_1 = 0x0004;

// operation kinds
const uint16_t alu = 0x6000;
const uint16_t immediate = 0x8000;
const uint16_t ubranch = 0x0000;
const uint16_t zero_branch = 0x2000;
const uint16_t scall = 0x4000;
}

struct BinaryJitT {
  void imm(uint16_t val) {
    ins.push_back(immediate | val);
    pos++;
  }
  void noop() {
    ins.push_back(alu | T);
    pos++;
  }
  void add() {
    ins.push_back(alu | T_plus_N | d_minus_1);
    pos++;
  }
  void lxor() {
    ins.push_back(alu | T_xor_N | d_minus_1);
    pos++;
  }
  void land() {
    ins.push_back(alu | T_and_N | d_minus_1);
    pos++;
  }
  void lor() {
    ins.push_back(alu | T_or_N | d_minus_1);
    pos++;
  }
  void invert() {
    ins.push_back(alu | invert_T);
    pos++;
  }
  void equal() {
    ins.push_back(alu | N_equal_T | d_minus_1);
    pos++;
  }
  void ult() {
    ins.push_back(alu | N_ult_T | d_minus_1);
    pos++;
  }
  void lt() {
    ins.push_back(alu | N_lt_T | d_minus_1);
    pos++;
  }
  void swap() {
    ins.push_back(alu | N | T_to_N);
    pos++;
  }
  void dup() {
    ins.push_back(alu | T | T_to_N | d_plus_1);
    pos++;
  }
  void drop() {
    ins.push_back(alu | N | d_minus_1);
    pos++;
  }
  void over() {
    ins.push_back(alu | N | T_to_N | d_plus_1);
    pos++;
  }
  void nip() {
    ins.push_back(alu | T | d_minus_1);
    pos++;
  }
  void to_r() {
    ins.push_back(alu | N | T_to_R | r_plus_1 | d_minus_1);
    pos++;
  }
  void from_r() {
    ins.push_back(alu | rT | T_to_N | r_minus_1 | d_plus_1);
    pos++;
  }
  void at_r() {
    ins.push_back(alu | rT | T_to_N | d_plus_1);
    pos++;
  }
  void at() {
    ins.push_back(alu | read_mem_T);
    pos++;
  }
  void at_io() {
    ins.push_back(alu | read_io_T);
    pos++;
  }
  void to_io() {
    ins.push_back(alu | T | N_to_ioT | d_minus_1);
    ins.push_back(alu | N | d_minus_1);
    pos++;
  }
  void to() {
    ins.push_back(alu | T | N_to_memT | d_minus_1);
    ins.push_back(alu | N | d_minus_1);
    pos++;
  };
  void shiftr() {
    ins.push_back(alu | N_shiftr_T | d_minus_1);
    pos++;
  }
  void shiftl() {
    ins.push_back(alu | N_shiftl_T | d_minus_1);
    pos++;
  }
  void exit() {
    ins.push_back(alu | T | RET | r_minus_1 | alu);
    pos++;
  }
  void jump(uint16_t target) {
    ins.push_back(ubranch | target);
    pos++;
  }

  void jump(const Label l) {
    ins.push_back(ubranch | l.pos);
    pos++;
  }

  void conditional_jump(uint16_t target) {
    ins.push_back(zero_branch | target);
    pos++;
  }

  void conditional_jump(const Label l) {
    ins.push_back(zero_branch | l.pos);
    pos++;
  }

  void label(Label &l) { l.pos = pos; }

  std::vector<uint16_t> ins;
  uint32_t pos;
};
}
}
