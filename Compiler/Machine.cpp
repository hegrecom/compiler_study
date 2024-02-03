#include "Machine.h"
#include "Instruction.h"
#include <any>

using std::any;
using std::get;

struct StackFrame {
  vector<any> variables;
  vector<any> operandStack;
  size_t instructionPointer;
};

static vector<StackFrame> callStack;

auto execute(tuple<vector<Code>, map<string, size_t>> objectCode) -> void {
  callStack.emplace_back();
  auto codeList = get<0>(objectCode);
  auto functionTable = get<1>(objectCode);
  while (true) {
    auto code = codeList[callStack.back().instructionPointer];
    switch (code.instruction) {
    case Instruction::Exit:
      callStack.pop_back();
      return;
    }
    callStack.back().instructionPointer++;
  }
}
