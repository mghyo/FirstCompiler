#include "IR.h"
#include <vector>
#include <map>
#include <set>

class Block;
class Function;

class Program {
 public:
  std::vector<Function *> functions;

  bool IsGlobal(const std::string &var) const;
  std::vector<std::string> GetGlobalInts() const;
};

class Function {
 public:
  std::string name;
  std::vector<std::string> intparams;
  std::vector<std::string> intlist, floatlist;
  Block *start = nullptr;

  bool isInt(const std::string &var) const;
  bool isFloat(const std::string &var) const;
  bool isVar(const std::string &var) const;
  bool IsVariableUsed(const std::string &var) const;
};

class Block {
 public:
  std::string label;
  std::vector<IRInstruction> ins;
  Block *after = nullptr; // in the code, which block is physically after this one
  std::vector<Block *> next; // possible next blocks in the cfg
  std::vector<Block *> prev; // previous blocks in the cfg
  std::map<int, std::string> defs;
  std::multimap<int, std::string> uses;
  std::set<std::string> liveout;

  Block() = default;
  Block(const std::string &_label) : label(_label) { }

  std::set<std::string> GetUses() const;
  std::set<std::string> GetDefs() const;
};

extern Block *createCfg(const std::vector<IRInstruction> &instructions);
