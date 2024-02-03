#include "Machine.h"
#include "DataType.h"
#include "Instruction.h"
#include "Kind.h"
#include <any>
#include <functional>
#include <iostream>

using std::any;
using std::cout;
using std::endl;
using std::function;
using std::get;

struct StackFrame {
  vector<any> variables;
  vector<any> operandStack;
  size_t instructionPointer;
};

static vector<StackFrame> callStack;
static map<string, any> global;
extern map<string, function<any(vector<any>)>> builtinFunctionTable;

static auto pushOperand(any) -> void;
static auto popOperand() -> any;
static auto peekOperand() -> any;

auto execute(tuple<vector<Code>, map<string, size_t>> objectCode) -> void {
  callStack.emplace_back();
  auto codeList = get<0>(objectCode);
  auto functionTable = get<1>(objectCode);
  while (true) {
    auto code = codeList[callStack.back().instructionPointer];
    switch (code.instruction) {
    case Instruction::Exit: {
      callStack.pop_back();
      return;
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
      }
      if (isBuiltinFunction(operand)) {
        vector<any> arguments;
        for (auto i = 0; i < toSize(code.operand); i++) {
          arguments.push_back(popOperand());
        }
        pushOperand(toBuiltinFunction(operand)(arguments));
        break;
      }
      pushOperand(nullptr);
      break;
    }
    case Instruction::Alloca: {
      auto extraSize = toSize(code.operand);
      auto currentSize = callStack.back().variables.size();
      callStack.back().variables.resize(currentSize + extraSize);
      break;
    }
    case Instruction::Return: {
      any result = nullptr;
      if (callStack.back().operandStack.empty() == false)
        result = callStack.back().operandStack.back();
      callStack.pop_back();
      callStack.back().operandStack.push_back(result);
      break;
    }
    case Instruction::Jump: {
      callStack.back().instructionPointer = toSize(code.operand);
      continue;
    }
    case Instruction::ConditionJump: {
      auto value = popOperand();
      if (isTrue(value))
        break;
      else {
        callStack.back().instructionPointer = toSize(code.operand);
        continue;
      }
    }
    case Instruction::Print: {
      for (auto i = 0; i < toSize(code.operand); i++) {
        auto value = popOperand();
        cout << value;
      }
      break;
    }
    case Instruction::PrintLine: {
      cout << endl;
      break;
    }
    case Instruction::LogicalOr: {
      auto value = popOperand();
      if (isTrue(value)) {
        pushOperand(value);
        callStack.back().instructionPointer = toSize(code.operand);
        continue;
      }
      break;
    }
    case Instruction::LogicalAnd: {
      auto value = popOperand();
      if (isFalse(value)) {
        pushOperand(value);
        callStack.back().instructionPointer = toSize(code.operand);
        continue;
      }
      break;
    }
    case Instruction::Add: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) + toNumber(rValue));
      else if (isString(lValue) && isString(rValue))
        pushOperand(toString(lValue) + toString(rValue));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::Subtract: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) - toNumber(rValue));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::Multiply: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) * toNumber(rValue));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::Divide: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue) && toNumber(rValue) == 0)
        pushOperand(0.0);
      else if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) / toNumber(rValue));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::Modulo: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue) && toNumber(rValue) == 0)
        pushOperand(0.0);
      else if (isNumber(lValue) && isNumber(rValue))
        pushOperand(fmod(toNumber(lValue), toNumber(rValue)));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::GetLocal: {
      auto index = toSize(code.operand);
      pushOperand(callStack.back().variables[index]);
      break;
    }
    case Instruction::SetLocal: {
      auto index = toSize(code.operand);
      callStack.back().variables[index] = peekOperand();
      break;
    }
    case Instruction::GetGlobal: {
      auto name = toString(code.operand);
      if (functionTable.count(name))
        pushOperand(functionTable[name]);
      else if (builtinFunctionTable.count(name))
        pushOperand(builtinFunctionTable[name]);
      else if (global.count(name))
        pushOperand(global[name]);
      else
        pushOperand(nullptr);
      break;
    }
    case Instruction::SetGlobal: {
      auto name = toString(code.operand);
      global[name] = peekOperand();
      break;
    }
    case Instruction::PushNull: {
      pushOperand(nullptr);
      break;
    }
    case Instruction::PushBoolean:
    case Instruction::PushNumber:
    case Instruction::PushString: {
      pushOperand(code.operand);
      break;
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

static auto peekOperand() -> any {
  return callStack.back().operandStack.back();
}
