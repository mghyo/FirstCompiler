#include "CFG.h"
#include "Strategy.h"
#include "CodeGen.h"
#include <iostream>

void IntraBlock::performLivenessAnalysis() {
    Block *currBlock = this->func->start; // start at first block in function
    IRInstruction * currIns;
    while(currBlock != nullptr){
        for(int i = 0; i < currBlock->ins.size(); i++){
            currIns = &(currBlock->ins[i]);
            switch (currIns->op){
                case OP::assign:
                    // Variable assignment
                    if(currIns->arg3.empty()){
                        if(func->isVar(currIns->arg1)){
                            currBlock->defs[i] = currIns->arg1;
                        }
                        if(func->isVar(currIns->arg2)){
                            currBlock->uses.insert(std::make_pair(i, currIns->arg2));
                        } 
                    }
                        // Array assignment
                    else {
                        if(func->isVar(currIns->arg1)){
                            currBlock->defs[i] = currIns->arg1;
                        }
                        if(func->isVar(currIns->arg2)){
                            currBlock->uses.insert(std::make_pair(i, currIns->arg2));
                        }
                        if(func->isVar(currIns->arg3)){
                            currBlock->uses.insert(std::make_pair(i, currIns->arg3));
                        }
                    }
                    break;
                case OP::add:
                case OP::sub:
                case OP::mult:
                case OP::div:
                case OP::_and:
                case OP::_or:
                    if(func->isVar(currIns->arg1)){
                        currBlock->uses.insert(std::make_pair(i, currIns->arg1));
                    }
                    if(func->isVar(currIns->arg2)){
                        currBlock->uses.insert(std::make_pair(i, currIns->arg2));
                    }
                    if(func->isVar(currIns->arg3)){
                        currBlock->defs[i] = currIns->arg3;
                    }
                case OP::_goto:
                    // UNUSED, no def or use
                    break;
                case OP::breq:
                case OP::brneq:
                case OP::brlt:
                case OP::brgt:
                case OP::brgeq:
                case OP::brleq:
                    if(func->isVar(currIns->arg1)){
                        currBlock->uses.insert(std::make_pair(i, currIns->arg1));
                    }
                    if(func->isVar(currIns->arg2)){
                        currBlock->uses.insert(std::make_pair(i, currIns->arg2));
                    } 
                    break;
                case OP::_return:
                    if(func->isVar(currIns->arg1)){
                        currBlock->uses.insert(std::make_pair(i, currIns->arg1));
                    }
                    break;
                case OP::call:
                    if(func->isVar(currIns->arg2)){
                        currBlock->uses.insert(std::make_pair(i, currIns->arg2));
                    }
                    if(func->isVar(currIns->arg3)){
                        currBlock->uses.insert(std::make_pair(i, currIns->arg3));
                    }
                    break;
                case OP::callr:
                    // TODO implement
                    break;
                case OP::array_store:
                    // TODO implement
                    break;
                case OP::array_load:
                    // TODO implement
                    break;
                default:
                    // not def or use
                    break;
            }
        }
        currBlock = currBlock->after; // go to next block
    }

    bool changed = true;
    while (changed) {
      changed = false;

      for (Block *block = func->start; block; block = block->after) {
        std::set<std::string> liveout;

        for (Block *succ : block->next) {
          std::set<std::string> lo = succ->liveout;
          // remove things we killed
          for (std::string kill : succ->GetDefs()) {
            lo.erase(kill);
          }

          // union with uses
          for (std::string use : succ->GetUses()) {
            lo.insert(use);
          }

          // union into running liveout
          for (std::string l : lo) liveout.insert(l);
        }
        if (block->liveout != liveout) {
          block->liveout = liveout; // apply
          changed = true;
        }
      }
    }
}

void IntraBlock::process(Program *program, Function *cfg) {
    this->program = program;
    this->func = cfg;
    //perform liveness analysis here
    performLivenessAnalysis();

}

void IntraBlock::enterBlock(Block *block) {
  // load all variables that this block uses, based on number of usages...
  std::map<std::string, int> uses; // variable -> usage count

  for (auto &it : block->uses) {
    int insidx = it.first;
    const std::string &variable = it.second;
    uses[variable]++;
  }

  std::multimap<int, std::string> rev;
  for (auto &it : uses) {
    rev.insert(std::make_pair(it.second, it.first));
  }

  assignments.clear();
  int idx = 0;
  Naive n;
  n.process(program, func);
  emit("# start of block - loading into registers");
  for (auto it = rev.rbegin(); it != rev.rend() && idx < MAX_REGISTERS; ++it) {
    emit("# variable " + it->second + " is assigned register $s" + std::to_string(idx));
    assignments[it->second] = idx;

    n.reg(it->second, "$s" + std::to_string(idx));

    ++idx;
  }
}

void IntraBlock::exitBlock(Block *block) {
  // store variables in registers back to the stack
  // note that we only have to store variables which got changed!
  
  std::set<std::string> defs = block->GetDefs();

  emit("# begin spilling");

  Naive n;
  n.process(program, func);
  for (auto &it : assignments) {
    const std::string &variable = it.first;
    if (defs.count(variable) == 0) continue;
    if (block->liveout.count(variable) == 0) continue;
    int _register = it.second;
    n.store("$s" + std::to_string(_register), variable); 
  }

  emit("# end of block");
}

int IntraBlock::numVariables() {
    return func->intlist.size() + func->floatlist.size();
}

std::string IntraBlock::reg(const std::string &variable, const std::string &suggestion) {
  if (assignments.find(variable) != assignments.end()) {
    if (suggestion.find("$a") == 0 || suggestion.find("$v") == 0) {
      // if the target is a function parameter or return value, then we just do a move
      emit("move", suggestion, "$s" + std::to_string(assignments[variable]), "# move of " + variable + " to fn arg/ret");
    }

    // there is a register assigned, so this is already valid and in a register
    return "$s" + std::to_string(assignments[variable]);
  }

  Naive n;
  n.process(program, func);
  return n.reg(variable, suggestion);
}

void IntraBlock::store(const std::string &reg, const std::string &variable) {
  if (assignments.find(variable) != assignments.end()) {
    if (reg[0] != '$') {
      // not a register
      emit("li", "$s" + std::to_string(assignments[variable]), reg);
    } else {
      emit("move", "$s" + std::to_string(assignments[variable]), reg);
    }
    return;
  }

  Naive n;
  n.process(program, func);
  return n.store(reg, variable);
}

void IntraBlock::emitAndStore(const std::string &op, const std::string &dest, const std::string &a2, const std::string &a3) {
  if (assignments.find(dest) != assignments.end()) {
    // dest variable already has a register
    emit(op, "$s" + std::to_string(assignments[dest]), a2, a3);
  } else {
    Naive n;
    n.process(program, func);
    return n.emitAndStore(op, dest, a2, a3);
  }
}
