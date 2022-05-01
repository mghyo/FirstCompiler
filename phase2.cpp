#include <iostream>
#include <fstream>
#include <vector>
#include "CFG.h"
#include "CodeGen.h"
#include "Strategy.h"
#include <sstream>

static inline void strip(std::string &str) {
  while (str.length() > 0 && (str[str.length() - 1] == ',' || str[str.length() - 1] == ' '
        || str[str.length() - 1] == '\t')) {
    str.erase(str.length() - 1);
  }
  while (str.length() > 0 && (str[0] == ',' || str[0] == ' ' || str[0] == '\t')) {
    str.erase(0, 1);
  }
  std::cout << str <<std::endl;
}

int main(int argc, char **argv) {

  if(argc != 3){
    std::cerr << "Usage   : ./phase2 <filename> <reg_alloc_scheme>" << std::endl;
    std::cerr << "Example : ./phase2 test/42.ir naive" << std::endl;
    return -1;
  }

  std::ifstream in(argv[1]);
  if (!in.is_open()) {
    std::cerr << "error opening file " << argv[1] << std::endl;
    return -1;
  }

  int reg_alloc_scheme = -1;

  if(std::string(argv[2]) == "naive"){
    reg_alloc_scheme = NAIVE;
  } else if(std::string(argv[2]) == "intra"){
    reg_alloc_scheme = INTRA;
  } else if(std::string(argv[2]) == "global"){
    reg_alloc_scheme = GLOBAL;
  } else{
    std::cerr << "Reg alloc scheme not supported." << std::endl;
    std::cerr << "Supported options are naive, intra, global" << std::endl;
    return -1;
  }


  std::vector<Function *> functions;
  Function *function = nullptr;
  std::vector<IRInstruction> instructions;
  std::string label;
  for (std::string line; std::getline(in, line);) {
    if (line.empty()) continue;

    if (line.find("#start_function") == 0) {
      function = new Function();

      function->name = line.substr(16);

      std::getline(in, line); // get signature
      // rettype fnname ( argtype argname )

      std::stringstream fnstream(line);
      std::string rettype, fnname, parambuf;

      std::getline(fnstream, rettype, ' ');
      std::getline(fnstream, fnname, '(');
      std::getline(fnstream, parambuf, ')');

      std::cout << " FUNCTION " << rettype << " " << fnname << " " << parambuf << std::endl;

      std::stringstream paramstrea(parambuf);
      while (std::getline(paramstrea, parambuf, ',')) {
        size_t idx = parambuf.find(' ');
        std::string type = parambuf.substr(0, idx);
        std::string name = parambuf.substr(idx+1);
        std::cout << " PARAM " << name << " " << type << std::endl;

        function->intparams.push_back(name);
     }

      continue;
    }

    if (line.find("#end_function") == 0) {
      Block *block = createCfg(instructions);
      function->start = block;

      functions.push_back(function);
      function = nullptr;

      instructions.clear();
      label.clear();
    }

    if (line.find("int-list: ") == 0) {
      std::stringstream ilist(line.substr(10));
      std::string buf;
      while (std::getline(ilist, buf, ',')) {
        strip(buf);
        std::cout << "INT " << buf << std::endl;
        function->intlist.push_back(buf);
        // std::cout << "INT " << buf << std::endl;
      }
      continue;
    }

    if (line.find("float-list: ") == 0) {
      std::stringstream ilist(line.substr(12));
      std::string buf;
      while (std::getline(ilist, buf, ',')) {
        strip(buf);
        function->floatlist.push_back(buf);
      }
      continue;
    }

    if (line.find(':') == line.length()-1) {
      label = line.substr(0, line.length() - 1);
      continue;
    }

    IRInstruction irins;
    try {
      irins = parseAsm(line);
    } catch( ...) {
      continue;
    }

    irins.label = label;
    label.clear();
    instructions.push_back(irins);
    // std::cout << "IrIns ---------------------------" << std::endl;
    // std::cout << irins << std::endl;
    // std::cout << "End IrIns ---------------------------" << std::endl;

  }

  in.close();

  std::stringstream out;
//  out << ".globl main" << std::endl;
  program = new Program();
  program->functions = functions;

  const std::vector<std::string> &globalInts = program->GetGlobalInts();
  if (!globalInts.empty()) {
    out << ".data" << std::endl;
    for (const std::string &s : globalInts) {
      out << s << ": .word 0" << std::endl;
    }
  }

  out << ".text" << std::endl;

  for (Function *function : functions) {
    switch(reg_alloc_scheme){
      case NAIVE:
        strat = new Naive();
        break;
      case INTRA:
        strat = new IntraBlock();
        break;
      case GLOBAL:
        strat = new Global();
        //return -1;
        break;
      default:
        // Should never get here
        std::cout << "Specify naive, intra, or global" << std::endl;
    }

    strat->process(program, function);
    out << function->name << ":" << std::endl;
    ::out = &out;
    generate(function, out);
    ::out = nullptr;
    // std::cout << "OUTPUT---------------------------" << std::endl;
    // std::cout << out.str() << std::endl;
  }

//  delete block;
//  delete function;

  std::ofstream outs("out.s");
  outs << out.str();
  outs.close();

  return 0;
}
