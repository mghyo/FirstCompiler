#include "IR.h"
#include <sstream>
#include <iostream>

static const char *optable[] = {
  "assign",
  "add",
  "sub",
  "mult",
  "div",
  "and",
  "or",
  "goto",
  "breq",
  "brneq",
  "brlt",
  "brgt",
  "brgeq",
  "brleq",
  "return",
  "call",
  "callr",
  "array_store",
  "array_load"
};

const char *op_to_str(OP op) {
  return optable[static_cast<int>(op)];
}

OP str_to_op(const std::string &op) {
  for (unsigned int i = 0; i < sizeof(optable)/sizeof(*optable); ++i) {
    if (op == optable[i]) {
      return static_cast<OP>(i);
    }
  }

  std::cerr << "unrecognized op: " << op << std::endl;
  throw std::invalid_argument(op);
}

static inline void strip(std::string &str) {
  while (str.length() > 0 && (str[str.length() - 1] == ',' || str[str.length() - 1] == ' '
        || str[str.length() - 1] == '\t')) {
    str.erase(str.length() - 1);
  }
  while (str.length() > 0 && (str[0] == ',' || str[0] == ' ' || str[0] == '\t')) {
    str.erase(0, 1);
  }
}

IRInstruction parseAsm(const std::string &line) {
  std::stringstream ss(line);
  std::string op1,op2,op3,op4;
  std::getline(ss, op1, ',');
  std::getline(ss, op2, ',');
  std::getline(ss, op3, ',');
  std::getline(ss, op4, ',');
  strip(op1);
  strip(op2);
  strip(op3);
  strip(op4);
  std::cout << line << std::endl;
  std::cout <<"." << op1 << ". " << op2 << " " <<op3 << " " << op4<<std::endl;
  OP op = str_to_op(op1);
  return IRInstruction(op, op2, op3, op4);
}

std::ostream& operator<<(std::ostream& os, const IRInstruction &irins) {
  os << op_to_str(irins.op) << " " << irins.arg1 << " " << irins.arg2 << " " << irins.arg3;
  return os;
}

bool IRInstruction::Label() const {
  return !label.empty();
}

const std::string *IRInstruction::Target() const {
  std::string targetLabel;

  switch (op) {
    case OP::_goto:
      return &arg1;
    case OP::breq:
    case OP::brneq:
    case OP::brlt:
    case OP::brgt:
    case OP::brgeq:
    case OP::brleq:
      return &arg3;
    default:
      return nullptr;
  }
}

bool IRInstruction::Terminal() const
{
  switch (op) {
    case OP::breq:
    case OP::brneq:
    case OP::brlt:
    case OP::brgt:
    case OP::brgeq:
    case OP::brleq:
    case OP::_return:
    case OP::_goto:
    case OP::call:
    case OP::callr:
      return true;
    default:
      return false;
  }
}

void IRInstruction::printIRInstruction() const{
  std::cout << "OP   : " << op_to_str(op) << std::endl;
  std::cout << "arg1 : " << arg1 << std::endl;
  std::cout << "arg2 : " << arg2 << std::endl;
  std::cout << "arg3 : " << arg3 << std::endl;
}
