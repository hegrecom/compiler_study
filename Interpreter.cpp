#include "Interpreter.h"
#include "DataType.h"
#include "Node.h"
#include <iostream>

using std::cout;
using std::endl;

static map<string, Function *> functionTable;

auto interpret(Program *program) -> void {
  for (auto &node : program->functions)
    functionTable[node->name] = node;

  if (functionTable["main"] == nullptr)
    return;

  functionTable["main"]->interpret();
}

auto Function::interpret() -> void {
  for (auto &node : block)
    node->interpret();
}

auto Return::interpret() -> void {}

auto Variable::interpret() -> void {}

auto For::interpret() -> void {}

auto Break::interpret() -> void {}

auto Continue::interpret() -> void {}

auto If::interpret() -> void {}

auto Print::interpret() -> void {
  for (auto &node : arguments) {
    auto value = node->interpret();
    cout << value;

    if (lineFeed)
      cout << endl;
  }
}

auto ExpressionStatement::interpret() -> void {}

auto Or::interpret() -> any { return nullptr; }

auto And::interpret() -> any { return nullptr; }

auto Relational::interpret() -> any { return nullptr; }

auto Arithmetic::interpret() -> any { return nullptr; }

auto Unary::interpret() -> any { return nullptr; }

auto Call::interpret() -> any { return nullptr; }

auto GetElement::interpret() -> any { return nullptr; }

auto SetElement::interpret() -> any { return nullptr; }

auto GetVariable::interpret() -> any { return nullptr; }

auto SetVariable::interpret() -> any { return nullptr; }

auto NullLiteral::interpret() -> any { return nullptr; }

auto BooleanLiteral::interpret() -> any { return nullptr; }

auto NumberLiteral::interpret() -> any { return nullptr; }

auto StringLiteral::interpret() -> any { return value; }

auto ArrayLiteral::interpret() -> any { return nullptr; }

auto MapLiteral::interpret() -> any { return nullptr; }
