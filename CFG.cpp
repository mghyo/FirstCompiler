#include "CFG.h"
#include <iostream>
#include <algorithm>

bool Program::IsGlobal(const std::string &var) const {
  if (var.empty() || var[0] == '$' || isdigit(var[0])) {
    return false;
  }

  int cnt = 0;
  for (Function *f : functions) {
    if (f->IsVariableUsed(var)) {
      ++cnt;
    }
  }
  return cnt > 1;
}

std::vector<std::string> Program::GetGlobalInts() const {
  std::vector<std::string> out;
  for (Function *f : functions) {
    for (const std::string &s : f->intlist) {
      if (IsGlobal(s)) {
        if (std::find(out.begin(), out.end(), s) == out.end()) {
          out.push_back(s);
        }
      }
    }
  }
  return out;
}

bool Function::isInt(const std::string &var) const {
  return std::find(intlist.begin(), intlist.end(), var) != intlist.end();
}

bool Function::isFloat(const std::string &var) const {
  return std::find(floatlist.begin(), floatlist.end(), var) != floatlist.end();
}

bool Function::isVar(const std::string &var) const {
  return isInt(var) || isFloat(var);
}

bool Function::IsVariableUsed(const std::string &var) const {
  // for extra fun, functions will use variables not in their int-list
  // which is what means that it is a global variable
  
  Block *b = start;
  while (b != nullptr) {
    for (const IRInstruction &ins : b->ins) {
      if (ins.arg1 == var || ins.arg2 == var || ins.arg3 == var) {
        return true;
      }
    }
    b = b->after;
  }

  return false;
}

static Block *find_block(const std::vector<Block *> &blocks, const std::string &name) {
  for (Block *block : blocks) {
    if (block->label == name) {
      return block;
    }
  }
  return nullptr;
}

Block *createCfg(const std::vector<IRInstruction> &instructions) {
  Block *block = nullptr;
  std::vector<Block *> blocks;
  bool newBlock = false;

  for (const IRInstruction &instruction : instructions) {
    if (block == nullptr) {
      // first block. assumed to have a label.
      block = new Block(instruction.label);
    } else if (instruction.Label() || newBlock) {
      blocks.push_back(block);
      Block *prev = block;
      block = new Block(instruction.label);
      prev->after = block;
      newBlock = false;
    }

    block->ins.push_back(instruction);

    if (instruction.Terminal()) {
      newBlock = true;
    }
  }

  blocks.push_back(block);

  int cnt = 0;
  for (Block *b : blocks) {
    std::cout << "Block " << (++cnt) << std::endl;
    for (const IRInstruction &i : b->ins)
      std::cout << i << std::endl;
  }

  for (Block *b : blocks) {
    const std::vector<IRInstruction> &ins = b->ins;
    if (ins.empty()) continue; // hmm

    const IRInstruction &last = ins[ins.size() - 1];
    // Anything other than a goto or return will fall through!
    if (last.op != OP::_return && last.op != OP::_goto) {
      Block *after = b->after;
      if (after != nullptr) {
        b->next.push_back(after);
        after->prev.push_back(b);
      }
    }

    const std::string *target = last.Target();
    if (target == nullptr) continue;

    // Find instruction with label target
    Block *targetBlock = find_block(blocks, *target);
    if (targetBlock == nullptr) {
      throw std::runtime_error("instruction targeting nonexistent label " + *target);
    }

    b->next.push_back(targetBlock);
    targetBlock->prev.push_back(b);
  }

  return blocks[0];
}

std::set<std::string> Block::GetUses() const {
  std::set<std::string> ret;
  for (auto &it : uses) {
    ret.insert(it.second);
  }
  return ret;
}

std::set<std::string> Block::GetDefs() const {
  std::set<std::string> ret;
  for (auto &it : defs) {
    ret.insert(it.second);
  }
  return ret;
}
