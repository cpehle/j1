#pragma once

#include <cstdint>

namespace dv {
namespace jit {

struct Label {
  uint32_t pos;
};

template <class JitBuilderT> struct JitBuilder {
  JitBuilder imm(uint16_t value) {
    jit_.imm(value);
    return JitBuilder{jit_};
  }
  JitBuilder add() {
    jit_.add();
    return JitBuilder{jit_};
  }
  JitBuilder land() {
    jit_.land();
    return JitBuilder{jit_};
  }
  JitBuilder lxor() {
    jit_.lxor();
    return JitBuilder{jit_};
  }
  JitBuilder lor() {
    jit_.lor();
    return JitBuilder{jit_};
  }
  JitBuilder invert() {
    jit_.invert();
    return JitBuilder{jit_};
  }
  JitBuilder equal() {
    jit_.equal();
    return JitBuilder{jit_};
  }
  JitBuilder ult() {
    jit_.ult();
    return JitBuilder{jit_};
  }
  JitBuilder lt() {
    jit_.lt();
    return JitBuilder{jit_};
  }
  JitBuilder swap() {
    jit_.swap();
    return JitBuilder{jit_};
  }
  JitBuilder dup() {
    jit_.dup();
    return JitBuilder{jit_};
  }
  JitBuilder drop() {
    jit_.drop();
    return JitBuilder{jit_};
  }
  JitBuilder over() {
    jit_.over();
    return JitBuilder{jit_};
  }
  JitBuilder nip() {
    jit_.nip();
    return JitBuilder{jit_};
  }
  JitBuilder to_r() {
    jit_.to_r();
    return JitBuilder{jit_};
  }
  JitBuilder from_r() {
    jit_.from_r();
    return JitBuilder{jit_};
  }
  JitBuilder at_r() {
    jit_.at_r();
    return JitBuilder{jit_};
  }
  JitBuilder at() {
    jit_.at();
    return JitBuilder{jit_};
  }
  JitBuilder at_io() {
    jit_.at_io();
    return JitBuilder{jit_};
  }
  JitBuilder to() {
    jit_.to();
    return JitBuilder{jit_};
  }
  JitBuilder to_io() {
    jit_.to_io();
    return JitBuilder{jit_};
  }
  JitBuilder exit() {
    jit_.exit();
    return JitBuilder{jit_};
  }
  JitBuilder shiftr() {
    jit_.shiftr();
    return JitBuilder{jit_};
  }
  JitBuilder shiftl() {
    jit_.shiftl();
    return JitBuilder{jit_};
  }
  JitBuilder noop() {
    jit_.noop();
    return JitBuilder{jit_};
  }
  JitBuilder jump(uint16_t target) {
    jit_.jump(target);
    return JitBuilder{jit_};
  }
  JitBuilder jump(const Label l) {
    jit_.jump(l);
    return JitBuilder{jit_};
  }
  JitBuilder label(Label& l) {
    jit_.label(l);
    return JitBuilder{jit_};
  }  
  JitBuilder conditional_jump(uint16_t target) {
    jit_.conditional_jump(target);
    return JitBuilder{jit_};
  }
  JitBuilder conditional_jump(const Label l) {
    jit_.conditional_jump(l);
    return JitBuilder{jit_};
  }
  
  /*
  :: 2dupand   T&N   T->N          d+1 alu ;
:: 2dup<     N<T   T->N          d+1 alu ;
:: 2dup=     N==T  T->N          d+1 alu ;
:: 2dupor    T|N   T->N          d+1 alu ;
:: 2duprshift N>>T T->N          d+1 alu ;
:: 2dup+     T+N   T->N          d+1 alu ;
:: 2dupu<    Nu<T  T->N          d+1 alu ;
:: 2dupxor   T^N   T->N          d+1 alu ;
:: dup>r     T     T->R      r+1     alu ;
:: dup@      [T]   T->N          d+1 alu ;
:: overand   T&N                     alu ;
:: over>     N<T                     alu ;
:: over=     N==T                    alu ;
:: overor    T|N                     alu ;
:: over+     T+N                     alu ;
:: overu>    Nu<T                    alu ;
:: overxor   T^N                     alu ;
:: rdrop     T                   r-1 alu ;
:: tuck!     T     N->[T]        d-1 alu ;
  */

  JitBuilderT jit_;
};
}
}
