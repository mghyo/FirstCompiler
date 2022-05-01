#include "CFG.h"
#include "Strategy.h"
#include <iostream>
#include <sstream>

Program *program = nullptr;
Strategy *strat = nullptr;
std::stringstream *out = nullptr;

static void generate(Function *function, Block *block, std::stringstream &out, IRInstruction &ins);
static void generate(Function *function, Block *block, std::stringstream &out);
static int syscall(const std::string &name);
static void branch(Function *function, IRInstruction &ins, const std::string &op, const std::string &comment);
static void arith(Function *function, IRInstruction &ins, const std::string &op);

void emit(const std::string &op) {
  *out << op << std::endl;
}

void emit(const std::string &op, const std::string &a1) {
  *out << op << ", " << a1 << std::endl;
}

void emit(const std::string &op, const std::string &a1, const std::string &a2) {
  *out << op << ", " << a1 << ", " << a2  << std::endl;
}

void emit(const std::string &op, const std::string &a1, const std::string &a2, const std::string &a3) {
  *out << op << ", " << a1 << ", " << a2 << ", " << a3 << std::endl;
}

void emit(const std::string &op, const std::string &a1, const std::string &a2, const std::string &a3, const std::string &comment) {
  *out << op << ", " << a1 << ", " << a2 << ", " << a3 << " " << comment << std::endl;
}

void generate(Function *function, std::stringstream &out) {
  strat->enterFunction();

  int numVariables = strat->numVariables() + 1; // for $ra
  emit("addiu", "$sp", "$sp", std::to_string(-4 * numVariables)); // allocate space on the stack for variables
  emit("sw", "$ra", std::to_string((numVariables - 1) * 4) + "($sp)"); // store $ra on stack

  // Copy function parameters to stack as they will get clobbered if this function calls another function
  // Note that the function parameters are also in int-list and so stack space has been assigned already
  int paramCnt = 0;
  for (const std::string &param : function->intparams) {
    strat->store("$a" + std::to_string(paramCnt++), param);
  }

  generate(function, function->start, out);
}

void generate(Function *function, Block *block, std::stringstream &out) {
  if (block->ins.empty() == false && block->ins[0].Label()) {
    IRInstruction &ins = block->ins[0];
    // functions have labels with the same name as their function name
    if (ins.label != function->name) {
      out << ins.label << ":" << std::endl;
    }
  }
  strat->enterBlock(block);
  for (IRInstruction &ins : block->ins) {
    generate(function, block, out, ins);
  }
  if (!block->ins.empty() && !block->ins[block->ins.size() - 1].Terminal()) {
    strat->exitBlock(block);
  }
  Block *next = block->after;
  if (next != nullptr) {
    generate(function, next, out);
  }
}

void generate(Function *function, Block *block, std::stringstream &out, IRInstruction &ins) {
  if (ins.Terminal()) {
    // this is the last instruction in the block! exit block to save stack before jumping
    strat->exitBlock(block);
  }
  switch (ins.op) {
    case OP::assign:
    {
      // arg1 = arg2

      if(ins.arg3 == "") {
        std::string a2;
        if (program->IsGlobal(ins.arg2)) {
          a2 = strat->reg(ins.arg2, "$t0");
        } else if (function->isInt(ins.arg2)) {
          // get register for arg2
          a2 = strat->reg(ins.arg2, "$t0");
        } else if (function->isFloat(ins.arg2)) {
          a2 = strat->reg(ins.arg2, "$f0");
        } else {
          a2 = ins.arg2;
        }

        // store into arg1
        strat->store(a2, ins.arg1);
      }
      // var X : ArrayInt := 10; /* ArrayInt is an int array of size 100 */
      //assign, X, 100, 10
      else {
        // Assign array
        // int size = atoi(ins.arg2.c_str());
        const std::string &arr = ins.arg1;
        const std::string &length = ins.arg2;
        const std::string &val = ins.arg3;

        if(true) { // This will be immediate
          emit("la", "$t0", arr);
          emit("li", "$t1", val);
          int size = atoi(length.c_str());
          for(int i = 0; i < size; i++){
            emit("sw", "$t1", std::to_string(4*i) + "($t0)");
          }
        } else if (false){ // this will be from var
          // TODO implement
        } else { // this will be float
          // TODO implement
        }

      }

      break;
    }
    case OP::add:
      arith(function, ins, "add");
      break;
    case OP::sub:
      arith(function, ins, "sub");
      break;
    case OP::mult:
      arith(function, ins, "mul");
      break;
    case OP::div:
      arith(function, ins, "div");
      break;
    case OP::_and:
      arith(function, ins, "and");
      break;
    case OP::_or:
      arith(function, ins, "or");
      break;
    case OP::_goto:
      emit("j", *ins.Target());
      break;
    case OP::breq:
      branch(function, ins, "beq", "# if (" + ins.arg1 + " == " + ins.arg2 + ") goto " + ins.arg3);
      break;
    case OP::brneq:
      branch(function, ins, "bne", "# if (" + ins.arg1 + " != " + ins.arg2 + ") goto " + ins.arg3);
      break;
    case OP::brlt:
      branch(function, ins, "blt", "# if (" + ins.arg1 + " < " + ins.arg2 + ") goto " + ins.arg3);
      break;
    case OP::brgt:
      branch(function, ins, "bgt", "# if (" + ins.arg1 + " > " + ins.arg2 + ") goto " + ins.arg3);
      break;
    case OP::brgeq:
      branch(function, ins, "bge", "# if (" + ins.arg1 + " >= " + ins.arg2 + ") goto " + ins.arg3);
      break;
    case OP::brleq:
      branch(function, ins, "ble", "# if (" + ins.arg1 + " <= " + ins.arg2 + ") goto " + ins.arg3);
      break;
    case OP::_return:
    {
      int numVariables = strat->numVariables() + 1; // for $ra
      // store result into $v0
      if (!ins.arg1.empty()) {
        if (function->isInt(ins.arg1)) {
          strat->reg(ins.arg1, "$v0"); // load variable into $v0
        } else {
          emit("li", "$v0", ins.arg1);
        }
      }

      emit("lw", "$ra", std::to_string((numVariables - 1) * 4) + "($sp)"); // load $ra from stack
      emit("addiu", "$sp", "$sp", std::to_string(4 * numVariables)); // remove space from stack
      emit("jr", "$ra"); // jump to return address
      break;
    }
    case OP::call:
    {
      // arg1 = function, arg2/arg3 arguments
      const std::string &func = ins.arg1;
      // load arguments into a0-a3. we only support two arguments though.
      const std::string &arg1 = ins.arg2;
      const std::string &arg2 = ins.arg3;
      bool isfloat = func == "printf"; // XXX

      if (!arg1.empty()) strat->reg(arg1, isfloat ? "$f12" : "$a0");
      if (!arg2.empty()) strat->reg(arg2, isfloat ? "$f12" : "$a1");

      int syscallId = syscall(func);
      if (syscallId != -1) {
        emit("li", "$v0", std::to_string(syscallId));
        emit("syscall", "# " + func);
      } else {
        strat->spill(block, &ins);
        emit("jal", func);
        strat->unspill(block, &ins);
      }
      break;
    }
    case OP::callr:
    {
      // arg1 = return variable, arg2 = function, arg3 = argument
      const std::string &ret = ins.arg1;
      const std::string &func = ins.arg2;
      const std::string &arg1 = ins.arg3;
      bool isfloat = func == "printf"; // XXX

      if (!arg1.empty()) strat->reg(arg1, isfloat ? "$f12" : "$a0");

      int syscallId = syscall(func);
      if (syscallId != -1) {
        emit("li", "$v0", std::to_string(syscallId));
        emit("syscall", "# " + func);
      } else {
        strat->spill(block, &ins);
        emit("jal", func);
        strat->unspill(block, &ins);
      }

      // result is in $v0
      strat->store("$v0", ret);
      break;
    }
    case OP::array_store: {
      const std::string &arr = ins.arg1;
      const std::string &offset = ins.arg2;
      const std::string &x = ins.arg3;
      emit("# start array store ");
      emit("la", "$t0", x);
      emit("lw", "$t1", "0($t0)");
      emit("la", "$t0", offset);
      emit("lw", "$t2", "0($t0)");
      emit("sll", "$t2", "$t2", "2");
      emit("la", "$t0", arr);
      emit("add" "$t0", "$t0", "$t2");
      emit("srl", "$t2", "$t2", "2");
      emit("sw", "$t1", "0($t0)");
      emit("# end array store ");
      break;
    }
    case OP::array_load: {
        const std::string &x = ins.arg1;
        const std::string &arr = ins.arg2;
        const std::string &offset = ins.arg3;
        emit("# start array load ");
        emit("la", "$t0", offset);
        emit("lw", "$t1", "0($t0)");
        emit("sll", "$t1", "$t1", "2");
        emit("la", "$t0", arr);
        emit("add", "$t0", "$t0", "$t1");
        emit("srl", "$t1", "$t1", "2");
        emit("lw", "$t1", "0($t0)");
        emit("la","$t0",x);
        emit("sw", "$t1", "0($t0)");
        emit("# end array load ");
        break;
    }
  }
}

static void branch(Function *function, IRInstruction &ins, const std::string &op, const std::string &comment) {
  // if (arg1 <= arg2) goto arg3;
  std::string a1;
  if (function->isInt(ins.arg1)) {
    a1 = strat->reg(ins.arg1, "$t0");
  } else {
    // arg1 must be a register, so we li the constant into it
    emit("li", "$t0", ins.arg1);
    a1 = "$t0";
  }

  std::string a2;
  if (function->isInt(ins.arg2)) {
    a2 = strat->reg(ins.arg2, "$t1");
  } else {
    a2 = ins.arg2;
  }

  std::string label = ins.arg3;
  emit(op, a1, a2, label, comment);
}

static void arith(Function *function, IRInstruction &ins, const std::string &op) {
  std::string a1;
  bool isfloat = false;
  if (function->isInt(ins.arg1)) {
    a1 = strat->reg(ins.arg1, "$t0");
  } else if (function->isFloat(ins.arg1)) {
    a1 = strat->reg(ins.arg1, "$f0");
    isfloat = true;
  } else {
    // arg1 must be a register, so we li the constant into it
    if (ins.arg1.find('.') != std::string::npos) {
      emit("li.s", "$f0", ins.arg1);
      a1 = "$f0";
      isfloat = true;
    } else {
      emit("li", "$t0", ins.arg1);
      a1 = "$t0";
    }
  }

  std::string a2;
  if (function->isInt(ins.arg2)) {
    a2 = strat->reg(ins.arg2, "$t1");
  } else if (function->isFloat(ins.arg2)) {
    a2 = strat->reg(ins.arg2, "$f2");
    isfloat = true;
  } else {
    if (ins.arg2.find('.') != std::string::npos) {
      // there is no pseudo instruction for add for floats that takes an immediate
      emit("li.s", "$f2", ins.arg2);
      a2 = "$f2";
      isfloat = true;
    } else {
      a2 = ins.arg2;
    }
  }

  std::string opname = op;
  if (isfloat) {
    opname += ".s";
  }

  strat->emitAndStore(opname, ins.arg3, a1, a2);
}


static int syscall(const std::string &name) {
  if (name == "printi") return 1;
  if (name == "printf") return 2;
  return -1;
}
