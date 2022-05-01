#include <iostream>
#include <algorithm>
#include <stack>

#include "CFG.h"
#include "Strategy.h"
#include "CodeGen.h"

void Global::performLivenessAnalysis() {
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
                            currIns->defs.insert(currIns->arg1);
                        }
                        if(func->isVar(currIns->arg2)){
                            currIns->uses.insert(currIns->arg2);
                        }
                    }
                        // Array assignment
                    else {
                        if(func->isVar(currIns->arg1)){
                            currIns->defs.insert(currIns->arg1);
                        }
                        if(func->isVar(currIns->arg2)){
                            currIns->uses.insert(currIns->arg2);
                        }
                        if(func->isVar(currIns->arg3)){
                            currIns->uses.insert(currIns->arg3);
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
                        currIns->uses.insert(currIns->arg1);
                    }
                    if(func->isVar(currIns->arg2)){
                        currIns->uses.insert(currIns->arg2);
                    }
                    if(func->isVar(currIns->arg3)){
                        currIns->defs.insert(currIns->arg3);
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
                        currIns->uses.insert(currIns->arg1);
                    }
                    if(func->isVar(currIns->arg2)){
                        currIns->uses.insert(currIns->arg2);
                    }
                    break;
                case OP::_return:
                    if(func->isVar(currIns->arg1)){
                        currIns->uses.insert(currIns->arg1);
                    }
                    break;
                case OP::call:
                    if(func->isVar(currIns->arg2)){
                        currIns->uses.insert(currIns->arg2);
                    }
                    if(func->isVar(currIns->arg3)){
                        currIns->uses.insert(currIns->arg3);
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

    // calculate in / out
    bool changed = true;
    while (changed) {
      changed = false;

      for (Block *block = func->start; block; block = block->after) {
        for (unsigned int idx = 0; idx < block->ins.size(); ++idx) {
          IRInstruction &ins = block->ins[idx];

          std::vector<IRInstruction *> next;
          if (idx + 1 == block->ins.size()) {
            for (Block *succ : block->next) {
              next.push_back(&succ->ins[0]);
            }
          } else {
            next.push_back(&block->ins[idx + 1]);
          }

          // liveout(n) = for each m in succ(n) : (uevar(m) u (liveout(m) - varkill(m)))
          std::set<std::string> liveout;
          for (IRInstruction *succ : next) {
            std::set<std::string> lo = succ->liveout;
            // remove things we killed
            for (std::string kill : succ->defs) {
              lo.erase(kill);
            }

            // union with uses
            for (std::string use : succ->uses) {
              lo.insert(use);
            }

            // union into running liveout
            for (std::string l : lo) liveout.insert(l);
          }
          if (ins.liveout != liveout) {
            ins.liveout = liveout; // apply
            changed = true;
          }
        }
      }
    }

#if 0
  std::cout << "LIVEOUT " << func->name << std::endl;
  for (Block *block = func->start; block; block = block->after) {
    std::cout << "BLOCK LIVEOUT " << block << std::endl;
    for (std::string l : block->liveout) {
      std::cout << " " << l << std::endl;
    }
    std::cout << "BLOCK USES" << std::endl;
    for (std::string u : block->GetUses()) {
      std::cout << " " <<u << std::endl;
    }
    std::cout << "BLOCK DEFS" << std::endl;
    for (std::string u : block->GetDefs()) {
      std::cout << " "<< u << std::endl;
    }
    std::cout << "BLOCK INS" << std::endl;
    for (IRInstruction &ins : block->ins) {
      std::cout << op_to_str(ins.op) << std::endl;
    }
  }
#endif
}

void Global::buildWebs() {
  webs.clear();

  for (Block *block = func->start; block; block = block->after) {
    for (IRInstruction &ins : block->ins) {
      for (std::string variable : ins.liveout) {
        if (program->IsGlobal(variable)) continue; // global variables don't get assigned registers

        Web* &w = webs[variable]; // each variable gets a web
        if (w == nullptr) {
          w = new Web();
          w->name = variable;
        }
        w->ins.insert(&ins);
      }
    }
  }
}

void Global::buildInterferenceGraph() {
  // each vertex in the graph is a web
  // each edge is an interference
  for (auto &it : webs) {
    Web *w1 = it.second;

    for (auto &it2 : webs) {
      Web *w2 = it2.second;

      if (w1 == w2) continue;

      // test if webs intersect
      std::vector<IRInstruction *> out(w1->ins.size() + w2->ins.size());
      std::set_intersection(w1->ins.begin(), w1->ins.end(),
          w2->ins.begin(), w2->ins.end(), out.begin());
      if (!out.empty()) {
        w1->edges.insert(w2);
        w2->edges.insert(w1);
        std::cout << "INTERFERENCE EDGE " << w1 << " " << w2 << std::endl;
      }
    }
  }
}

void Global::color() {
  std::vector<Web *> webs;
  for (auto &it : this->webs) webs.push_back(it.second);

  std::stack<Web *> stack;
  // remove nodes that have degree <n
  for (auto it = webs.begin(); it != webs.end();) {
    Web *web = *it;

    if (web->degree() < MAX_REGISTERS) {
      stack.push(web);
      it = webs.erase(it);
    } else {
      ++it;
    }
  }

  // remaining are only webs with degree >= n.
  // we're supposed to push them based on 'spill cost'
  for (Web *w : webs) {
    stack.push(w);
  }
  webs.clear();

  while (!stack.empty()) {
    Web *web = stack.top();
    stack.pop();

    // Color based on neighbors
    for (int i = 0; i < MAX_REGISTERS; ++i) {
      for (Web *other : web->edges) {
        if (other->color == i) {
          // color is already used
          goto cont;
        }
      }

      // assign color
      web->color = i;
      std::cout << " ASSIGN web " << web->name << " color " << i << std::endl;
      break;

cont:;
    }
  }
}

void Global::process(Program *program, Function *cfg) {
    this->program = program;
    this->func = cfg;

    // perform the full CFG liveness analysis across the basic blocks
    performLivenessAnalysis();

    // build live ranges and the webs
    buildWebs();

    // build an interference graph
    buildInterferenceGraph();

    // perform graph coloring using Briggs’ optimistic coloring algorithm
    color();
}

void Global::enterFunction() {
  emit("# enter " + func->name);
  for (auto &it : webs) {
    Web *web = it.second;
    if (web->color != -1)
      emit("# variable " + web->name + " assigned register " + std::to_string(web->color));
    else
      emit("# variable " + web->name + " is spilled!");
  }
}

void Global::spill(Block *block, IRInstruction *ins) {
  emit("# spilling for jal");
  Naive n;
  n.process(program, func);
  for (auto &it : webs) {
    Web *web = it.second;
    if (web->color != -1 && web->ins.count(ins))
      n.store("$s" + std::to_string(web->color), web->name);
  }
}

void Global::unspill(Block *block, IRInstruction *ins) {
  emit("# unspilling");
  Naive n;
  n.process(program, func);
  for (auto &it : webs) {
    Web *web = it.second;
    if (web->color != -1 && web->ins.count(ins))
      n.reg(web->name, "$s" + std::to_string(web->color));
  }
}

int Global::numVariables() {
    return func->intlist.size() + func->floatlist.size();
}

std::string Global::reg(const std::string &variable, const std::string &suggestion) {
  if (program->IsGlobal(variable)) {
    // global always gets loaded
    emit("lw", suggestion, variable, "# load from " + variable);
    return suggestion;
  }

  Web *web = webs.find(variable) != webs.end() ? webs[variable] : nullptr;
  if (web != nullptr && web->color != -1) {
    if (suggestion.find("$a") == 0 || suggestion.find("$v") == 0) {
      // if the target is a function parameter or return value, then we just do a move
      emit("move", suggestion, "$s" + std::to_string(web->color), "# move of " + variable + " to fn arg/ret");
    }

    // there is a register assigned, so this is already valid and in a register
    return "$s" + std::to_string(web->color);
  }

  Naive n;
  n.process(program, func);
  return n.reg(variable, suggestion);
}

void Global::store(const std::string &reg, const std::string &variable) {
  if (program->IsGlobal(variable)) {
    // global always gets stored
    Naive n;
    n.process(program, func);
    return n.store(reg, variable);
  }

  Web *web = webs.find(variable) != webs.end() ? webs[variable] : nullptr;
  if (web != nullptr && web->color != -1) {
    // this is assigned a register!
    if (reg[0] != '$') {
      // not a register
      emit("li", "$s" + std::to_string(web->color), reg, "# store to " + variable);
    } else {
      emit("move", "$s" + std::to_string(web->color), reg, "# store to " + variable);
    }
    return;
  }

  Naive n;
  n.process(program, func);
  return n.store(reg, variable);
}

void Global::emitAndStore(const std::string &op, const std::string &dest, const std::string &a2, const std::string &a3) {
  Web *web = webs.find(dest) != webs.end() ? webs[dest] : nullptr;
  if (web != nullptr && web->color != -1) {
    emit(op, "$s" + std::to_string(web->color), a2, a3);
    return;
  }

  Naive n;
  n.process(program, func);
  return n.emitAndStore(op, dest, a2, a3);
}

