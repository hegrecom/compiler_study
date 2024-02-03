#include "Machine.h"
#include "DataType.h"
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
static auto popOperand() -> any;

auto execute(tuple<vector<Code>, map<string, size_t>> objectCode) -> void {
  callStack.emplace_back();
  auto codeList = get<0>(objectCode);
  auto functionTable = get<1>(objectCode);
  while (true) {
    auto code = codeList[callStack.back().instructionPointer];
    switch (code.instruction) {
    case Instruction::Alloca: {
      auto extraSize = toSize(code.operand);
      auto currentSize = callStack.back().variables.size();
      callStack.back().variables.resize(currentSize + extraSize);
      break;
    }
    case Instruction::Call: {
      auto operand = popOperand();
      if (isSize(operand)) {
        StackFrame stackFrame;
        stackFrame.instructionPointer = toSize(operand);
        for (auto i = 0; i < toSize(code.operand); i++) {
          stackFrame.variables.push_back(callStack.back().operandStack.back());
          callStack.back().operandStack.pop_back();
        }
        callStack.push_back(stackFrame);
        continue;
      } else {
        pushOperand(nullptr);
      }
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

static auto popOperand() -> any {
  auto value = callStack.back().operandStack.back();
  callStack.back().operandStack.pop_back();
  return value;
}
