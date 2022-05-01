#include <string>
#include <map>
#include <set>

#define MAX_REGISTERS 8

#define NAIVE 0
#define INTRA 1
#define GLOBAL 2

class Function;
class Program;
class IRInstruction;
class Block;

class Strategy {
 public:
  virtual void process(Program *program, Function *function) = 0;
  virtual void enterFunction() { }
  virtual void enterBlock(Block *block) { }
  virtual void exitBlock(Block *block) { }
  virtual void spill(Block *, IRInstruction *) { }
  virtual void unspill(Block *, IRInstruction *) { }
  virtual int numVariables() = 0;
  virtual std::string reg(const std::string &variable, const std::string &suggestion) = 0;
  virtual void store(const std::string &reg, const std::string &variable) = 0;
  virtual void emitAndStore(const std::string &op, const std::string &dest, const std::string &a2, const std::string &a3) = 0;
};

class Naive : public Strategy {
 public:
  Program *program = nullptr;
  Function *func = nullptr;
  void process(Program *, Function *cfg) override;
  int numVariables() override;
  std::string reg(const std::string &variable, const std::string &suggestion) override;
  void store(const std::string &reg, const std::string &variable) override;
  void emitAndStore(const std::string &op, const std::string &dest, const std::string &a2, const std::string &a3) override;
};

class IntraBlock : public Strategy{
 public:
  std::map<std::string, int> assignments; // variable -> what register assignment
  Program *program = nullptr;
  Function *func = nullptr;
  void performLivenessAnalysis();
  void process(Program *, Function *cfg) override;
  void enterBlock(Block *block) override;
  void exitBlock(Block *block) override;
  int numVariables() override;
  std::string reg(const std::string &variable, const std::string &suggestion) override;
  void store(const std::string &reg, const std::string &variable) override;
  void emitAndStore(const std::string &op, const std::string &dest, const std::string &a2, const std::string &a3) override;
};

class Global : public Strategy{
  class Web {
   public:
    std::string name; // variable name
    std::set<IRInstruction *> ins; // ins in this web
    std::set<Web *> edges; // interference graph edges
    int color = -1; // register color, through MAX_REGISTERS

    inline int degree() const { return edges.size(); }
  };
  std::map<std::string, Web *> webs;
 public:
    Program *program = nullptr;
    Function *func = nullptr;
    void performLivenessAnalysis();
    void buildWebs();
    void buildInterferenceGraph();
    void color();

    void process(Program *, Function *cfg) override;
    void enterFunction() override;
    void spill(Block *, IRInstruction *) override;
    void unspill(Block *, IRInstruction *) override;
    int numVariables() override;
    std::string reg(const std::string &variable, const std::string &suggestion) override;
    void store(const std::string &reg, const std::string &variable) override;
    void emitAndStore(const std::string &op, const std::string &dest, const std::string &a2, const std::string &a3) override;
};
