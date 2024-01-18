#include "Generator.h"
#include "Code.h"
#include "Instruction.h"
#include <any>
#include <cstddef>
#include <vector>

static vector<Code> codeList;
static map<string, size_t> functionTable;

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

auto Function::generate() -> void {
  functionTable[name] = codeList.size();
  for (auto &node : block)
    node->generate();
  writeCode(Instruction::Return);
}

auto Return::generate() -> void {}

auto Variable::generate() -> void {}

auto For::generate() -> void {}

auto Break::generate() -> void {}

auto Continue::generate() -> void {}

auto If::generate() -> void {}

auto Print::generate() -> void {
  for (auto i = arguments.size(); i > 0; i--)
    arguments[i - 1]->generate();
  writeCode(Instruction::Print, arguments.size());

  if (lineFeed)
    writeCode(Instruction::PrintLine);
}

auto ExpressionStatement::generate() -> void {}

auto Or::generate() -> void {}

auto And::generate() -> void {}

auto Relational::generate() -> void {}

auto Arithmetic::generate() -> void {}

auto Unary::generate() -> void {}

auto Call::generate() -> void {}

auto GetElement::generate() -> void {}

auto SetElement::generate() -> void {}

auto GetVariable::generate() -> void {}

auto SetVariable::generate() -> void {}

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
