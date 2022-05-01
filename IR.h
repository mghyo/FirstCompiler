#include <string>
#include <set>
enum class OP {
  assign,
  add,
  sub,
  mult,
  div,
  _and,
  _or,
  _goto,
  breq,
  brneq,
  brlt,
  brgt,
  brgeq,
  brleq,
  _return,
  call,
  callr,
  array_store,
  array_load,
};



const char *op_to_str(OP op);

class IRInstruction {
 public:
   OP op;
   std::string arg1, arg2, arg3;
   std::string label;

   IRInstruction() = default;
   IRInstruction(OP _op, const std::string &_arg1, const std::string &_arg2,
       const std::string &_arg3) : op(_op), arg1(_arg1), arg2(_arg2), arg3(_arg3) { }

   // Return true if this instruction has a label (is a jump target)
   bool Label() const;
   // Get the target label for this instruction, if it is a jump
   const std::string *Target() const;
   bool Terminal() const;

   void printIRInstruction() const;

   std::set<std::string> defs, uses, liveout;
};

extern IRInstruction parseAsm(const std::string &line);

std::ostream& operator<<(std::ostream& os, const IRInstruction &irins);
