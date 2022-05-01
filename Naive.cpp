#include "Strategy.h"
#include "CFG.h"
#include "CodeGen.h"
#include <algorithm>
#include <assert.h>

void Naive::process(Program *program, Function *func) {
  this->program = program;
  this->func = func;
}

int Naive::numVariables() {
  return func->intlist.size() + func->floatlist.size();
}

std::string Naive::reg(const std::string &variable, const std::string &suggestion) {
  std::string to;
  bool isfloat = false;
  if (program->IsGlobal(variable)) {
    to = variable; // there is a global symbol with this name
  } else if (std::find(func->intlist.begin(), func->intlist.end(), variable) != func->intlist.end()) {
    auto it = std::find(func->intlist.begin(), func->intlist.end(), variable);
    assert(it != func->intlist.end());
    int idx = std::distance(func->intlist.begin(), it);
    int offset = 4 * idx; // assume every variable is a word?
    to = std::to_string(offset) + "($sp)"; // load based on offset to sp
  } else {
    auto it = std::find(func->floatlist.begin(), func->floatlist.end(), variable);
    assert(it != func->floatlist.end());
    int idx = std::distance(func->floatlist.begin(), it);
    // we offset from end of intlist
    int offset = (func->intlist.size() * 4) + (4 * idx);
    to = std::to_string(offset) + "($sp)"; // load based on offset to sp
    isfloat = true;
  }
  emit(isfloat ? "l.s" : "lw", suggestion, to, "# load from " + variable); // load from stack into $t0
  return suggestion;
}

void Naive::store(const std::string &reg, const std::string &variable) {
  std::string to;
  bool isfloat = false;
  if (program->IsGlobal(variable)) {
    to = variable;
  } else if (std::find(func->intlist.begin(), func->intlist.end(), variable) != func->intlist.end()) {
    auto it = std::find(func->intlist.begin(), func->intlist.end(), variable);
    assert(it != func->intlist.end());
    int idx = std::distance(func->intlist.begin(), it);
    int offset = 4 * idx; // assume every variable is a word?
    to = std::to_string(offset) + "($sp)";
  } else {
    auto it = std::find(func->floatlist.begin(), func->floatlist.end(), variable);
    assert(it != func->floatlist.end());
    int idx = std::distance(func->floatlist.begin(), it);
    // we offset from end of intlist
    int offset = (func->intlist.size() * 4) + (4 * idx);
    to = std::to_string(offset) + "($sp)";
    isfloat = true;
  }
  
  std::string r = reg;
  if (reg[0] != '$') {
    // we can't store an immediate to memory without putting it
    // into a temporary register first
    if (!isfloat) {
      emit("li", "$t0", reg);
      r = "$t0";
    } else {
      emit("li.s", "$f0", reg);
      r = "$f0";
    }
  }

  emit(isfloat ? "s.s" : "sw", r, to, "# store to " + variable); // store onto stack
}

void Naive::emitAndStore(const std::string &op, const std::string &dest, const std::string &a2, const std::string &a3) {
  bool isfloat = func->isFloat(dest);
  std::string to = isfloat ? "$f4" : "$t2";

  emit(op, to, a2, a3);

  store(to, dest);
}
