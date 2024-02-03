#include "Machine.h"
#include <any>
#include <vector>

using std::any;
using std::vector;

struct StackFrame {
  vector<any> variables;
  vector<any> operandStack;
  size_t instructionPointer;
};

static vector<StackFrame> callStack;

auto execute(vector<Code>, vector<Code>, vector<Code> objectCode) -> void {
  callStack.emplace_back();
}
