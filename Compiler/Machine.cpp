#include "Machine.h"
#include "Instruction.h"
#include "Kind.h"
#include <any>

using std::any;
using std::get;

struct StackFrame {
  vector<any> variables;
  vector<any> operandStack;
  size_t instructionPointer;
};

static vector<StackFrame> callStack;

static auto pushOperand(any) -> void;

auto execute(tuple<vector<Code>, map<string, size_t>> objectCode) -> void {
  callStack.emplace_back();
  auto codeList = get<0>(objectCode);
  auto functionTable = get<1>(objectCode);
  while (true) {
    auto code = codeList[callStack.back().instructionPointer];
    switch (code.instruction) {
    case Instruction::Call: {
      break;
    }
    case Instruction::GetGlobal: {
      auto name = toString(code.operand);
      if (functionTable.count(name))
        pushOperand(functionTable[name]);
      else
        pushOperand(nullptr);
      break;
    }
    case Instruction::Exit: {
      callStack.pop_back();
      return;
    }
    }
    callStack.back().instructionPointer++;
  }
}

static auto pushOperand(any operand) -> void {
  callStack.back().operandStack.push_back(operand);
}
