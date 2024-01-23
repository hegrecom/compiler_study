#include "Generator.h"
#include "Code.h"
#include "Instruction.h"
#include "Kind.h"
#include <algorithm>
#include <any>
#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

using std::list;
using std::max;

static vector<Code> codeList;
static map<string, size_t> functionTable;
static list<map<string, size_t>> symbolStack;
static vector<size_t> offsetStack;
static size_t localSize;
static vector<vector<size_t>> continueStack;

auto writeCode(Instruction) -> size_t;
auto writeCode(Instruction, any) -> size_t;

auto generate(Program *program) -> tuple<vector<Code>, map<string, size_t>> {
  writeCode(Instruction::GetGlobal, string("main"));
  writeCode(Instruction::Call, static_cast<size_t>(0));
  writeCode(Instruction::Exit);
  for (auto &node : program->functions)
    node->generate();

  return {codeList, functionTable};
}

auto writeCode(Instruction instruction) -> size_t {
  codeList.push_back({instruction});
  return codeList.size() - 1;
}

auto writeCode(Instruction instruction, any operand) -> size_t {
  codeList.push_back({instruction, operand});
  return codeList.size() - 1;
}

auto patchAddress(size_t codeIndex) -> void {
  codeList[codeIndex].operand = codeList.size();
}

auto patchOperand(size_t codeIndex, size_t operand) -> void {
  codeList[codeIndex].operand = operand;
}

auto initBlock() -> void {
  localSize = 0;
  offsetStack.push_back(0);
  symbolStack.emplace_front();
}

auto popBlock() -> void {
  offsetStack.pop_back();
  symbolStack.pop_front();
}

auto pushBlock() -> void {
  symbolStack.emplace_front();
  offsetStack.push_back(offsetStack.back());
}

auto setLocal(string name) -> void {
  symbolStack.front()[name] = offsetStack.back();
  offsetStack.back() += 1;
  localSize = max(localSize, offsetStack.back());
}

auto getLocal(string name) -> size_t {
  for (auto &symbolTable : symbolStack) {
    if (symbolTable.count(name))
      return symbolTable[name];
  }

  return SIZE_MAX;
}

auto Function::generate() -> void {
  functionTable[name] = codeList.size();
  auto temp = writeCode(Instruction::Alloca);
  initBlock();
  for (auto &name : parameters)
    setLocal(name);
  for (auto &node : block)
    node->generate();
  popBlock();
  patchOperand(temp, localSize);
  writeCode(Instruction::Return);
}

auto Return::generate() -> void {
  expression->generate();
  writeCode(Instruction::Return);
}

auto Variable::generate() -> void {
  setLocal(name);
  expression->generate();
  writeCode(Instruction::SetLocal, getLocal(name));
  writeCode(Instruction::PopOperand);
}

auto For::generate() -> void {
  continueStack.emplace_back();
  pushBlock();
  variable->generate();
  auto jumpAddress = codeList.size();
  condition->generate();
  auto conditionJump = writeCode(Instruction::ConditionJump);
  for (auto &node : block)
    node->generate();
  auto continueAddress = codeList.size();
  expression->generate();
  writeCode(Instruction::PopOperand);
  writeCode(Instruction::Jump, jumpAddress);
  patchAddress(conditionJump);
  popBlock();
  for (auto &jump : continueStack.back())
    patchOperand(jump, continueAddress);
  continueStack.pop_back();
}

auto Break::generate() -> void {}

auto Continue::generate() -> void {
  if (continueStack.empty())
    return;
  auto jumpCode = writeCode(Instruction::Jump);
  continueStack.back().push_back(jumpCode);
}

auto If::generate() -> void {
  vector<size_t> jumpList;
  for (auto i = 0; i < conditions.size(); i++) {
    conditions[i]->generate();
    auto conditionJump = writeCode(Instruction::ConditionJump);
    pushBlock();
    for (auto &node : blocks[i])
      node->generate();
    popBlock();
    jumpList.push_back(writeCode(Instruction::Jump));
    patchAddress(conditionJump);
  }
  if (elseBlock.empty() == false) {
    pushBlock();
    for (auto &node : elseBlock)
      node->generate();
    popBlock();
  }
  for (auto &jump : jumpList)
    patchAddress(jump);
}

auto Print::generate() -> void {
  for (auto i = arguments.size(); i > 0; i--)
    arguments[i - 1]->generate();
  writeCode(Instruction::Print, arguments.size());

  if (lineFeed)
    writeCode(Instruction::PrintLine);
}

auto ExpressionStatement::generate() -> void {
  expression->generate();
  writeCode(Instruction::PopOperand);
}

auto Or::generate() -> void {
  lhs->generate();
  auto logicalOr = writeCode(Instruction::LogicalOr);
  rhs->generate();
  patchAddress(logicalOr);
}

auto And::generate() -> void {
  lhs->generate();
  auto logicalAnd = writeCode(Instruction::LogicalAnd);
  rhs->generate();
  patchAddress(logicalAnd);
}

auto Relational::generate() -> void {}

auto Arithmetic::generate() -> void {
  map<Kind, Instruction> instructions = {
      {Kind::Add, Instruction::Add},
      {Kind::Subtract, Instruction::Subtract},
      {Kind::Multiply, Instruction::Multiply},
      {Kind::Divide, Instruction::Divide},
      {Kind::Modulo, Instruction::Modulo},
  };
  lhs->generate();
  rhs->generate();

  writeCode(instructions[kind]);
}

auto Unary::generate() -> void {}

auto Call::generate() -> void {
  for (auto i = arguments.size(); i > 0; i--)
    arguments[i - 1]->generate();
  sub->generate();
  writeCode(Instruction::Call, arguments.size());
}

auto GetElement::generate() -> void {}

auto SetElement::generate() -> void {}

auto GetVariable::generate() -> void {
  if (getLocal(name) == SIZE_MAX)
    writeCode(Instruction::GetGlobal, name);
  else
    writeCode(Instruction::GetLocal, getLocal(name));
}

auto SetVariable::generate() -> void {
  value->generate();
  if (getLocal(name) == SIZE_MAX)
    writeCode(Instruction::SetGlobal, name);
  else
    writeCode(Instruction::SetLocal, getLocal(name));
}

auto NullLiteral::generate() -> void {
  writeCode(Instruction::PushNull, nullptr);
}

auto BooleanLiteral::generate() -> void {
  writeCode(Instruction::PushBoolean, value);
}

auto NumberLiteral::generate() -> void {
  writeCode(Instruction::PushNumber, value);
}

auto StringLiteral::generate() -> void {
  writeCode(Instruction::PushString, value);
}

auto ArrayLiteral::generate() -> void {}

auto MapLiteral::generate() -> void {}
