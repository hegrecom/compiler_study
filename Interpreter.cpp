#include "Interpreter.h"
#include "DataType.h"
#include "Node.h"
#include <any>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <vector>

using std::cout;
using std::endl;
using std::function;
using std::list;
using std::make_shared;

static map<string, Function *> functionTable;
static list<list<map<string, any>>> local;
static map<string, any> global;
extern map<string, function<any(vector<any>)>> builtinFunctionTable;

struct ContinueException {};
struct BreakException {};
struct ReturnException {
  any result;
};

auto interpret(Program *program) -> void {
  for (auto &node : program->functions)
    functionTable[node->name] = node;

  if (functionTable["main"] == nullptr)
    return;

  try {
    local.emplace_back().emplace_front();
    functionTable["main"]->interpret();
  } catch (ReturnException e) {
    local.pop_back();
  }
}

auto Function::interpret() -> void {
  for (auto &node : block)
    node->interpret();
}

auto Return::interpret() -> void {
  throw ReturnException{expression->interpret()};
}

auto Variable::interpret() -> void {
  local.back().front()[name] = expression->interpret();
}

auto For::interpret() -> void {
  local.back().emplace_front();
  variable->interpret();

  while (true) {
    auto result = condition->interpret();
    if (isTrue(result) == false)
      break;
    try {
      for (auto &node : block)
        node->interpret();
    } catch (ContinueException e) {
    } catch (BreakException e) {
      break;
    }
    expression->interpret();
  }
  local.back().pop_front();
}

auto Break::interpret() -> void { throw BreakException(); }

auto Continue::interpret() -> void { throw ContinueException(); }

auto If::interpret() -> void {
  for (auto i = 0; i < conditions.size(); i++) {
    auto result = conditions[i]->interpret();
    if (isTrue(result) == false)
      continue;
    local.back().emplace_front();
    for (auto &node : blocks[i])
      node->interpret();
    local.back().pop_front();
    return;
  }

  if (elseBlock.empty())
    return;

  local.back().emplace_front();
  for (auto &node : elseBlock)
    node->interpret();
  local.back().pop_front();
}

auto Print::interpret() -> void {
  for (auto &node : arguments) {
    auto value = node->interpret();
    cout << value;
  }

  if (lineFeed)
    cout << endl;
}

auto ExpressionStatement::interpret() -> void { expression->interpret(); }

auto Or::interpret() -> any {
  return isTrue(lhs->interpret()) ? true : rhs->interpret();
}

auto And::interpret() -> any {
  return isFalse(lhs->interpret()) ? false : rhs->interpret();
}

auto Relational::interpret() -> any {
  auto lValue = lhs->interpret();
  auto rValue = rhs->interpret();

  if (kind == Kind::Equal && isNumber(lValue) && isNumber(rValue))
    return toNumber(lValue) == toNumber(rValue);
  if (kind == Kind::NotEqual && isNumber(lValue) && isNumber(rValue))
    return toNumber(lValue) != toNumber(rValue);
  if (kind == Kind::LessThan && isNumber(lValue) && isNumber(rValue))
    return toNumber(lValue) < toNumber(rValue);
  if (kind == Kind::GreaterThan && isNumber(lValue) && isNumber(rValue))
    return toNumber(lValue) > toNumber(rValue);
  if (kind == Kind::LessOrEqual && isNumber(lValue) && isNumber(rValue))
    return toNumber(lValue) <= toNumber(rValue);
  if (kind == Kind::GreaterOrEqual && isNumber(lValue) && isNumber(rValue))
    return toNumber(lValue) >= toNumber(rValue);

  return false;
}

auto Arithmetic::interpret() -> any {
  auto lValue = lhs->interpret();
  auto rValue = rhs->interpret();

  if (kind == Kind::Add && isNumber(lValue) && isNumber(rValue))
    return toNumber(lValue) + toNumber(rValue);
  if (kind == Kind::Subtract && isNumber(lValue) && isNumber(rValue))
    return toNumber(lValue) - toNumber(rValue);
  if (kind == Kind::Multiply && isNumber(lValue) && isNumber(rValue))
    return toNumber(lValue) * toNumber(rValue);
  if (kind == Kind::Divide && isNumber(lValue) && isNumber(rValue))
    return toNumber(rValue) == 0 ? 0.0 : toNumber(lValue) / toNumber(rValue);
  if (kind == Kind::Modulo && isNumber(lValue) && isNumber(rValue))
    return toNumber(rValue) == 0 ? toNumber(lValue)
                                 : fmod(toNumber(lValue), toNumber(rValue));
  return 0.0;
}

auto Unary::interpret() -> any {
  auto value = sub->interpret();

  if (kind == Kind::Add && isNumber(value))
    return toNumber(value);
  if (kind == Kind::Subtract && isNumber(value))
    return -toNumber(value);

  return nullptr;
}

auto Call::interpret() -> any {
  auto value = sub->interpret();
  if (isBuiltinFunction(value)) {
    vector<any> values;

    for (auto i = 0; i < arguments.size(); i++)
      values.push_back(arguments[i]->interpret());
    return toBuiltinFunction(value)(values);
  }

  if (isFunction(value) == false)
    return nullptr;
  map<string, any> parameters;
  for (auto i = 0; i < arguments.size(); i++) {
    auto name = toFunction(value)->parameters[i];
    parameters[name] = arguments[i]->interpret();
  }

  local.emplace_back().push_front(parameters);
  try {
    toFunction(value)->interpret();
  } catch (ReturnException exception) {
    local.pop_back();
    return exception.result;
  }
  local.pop_back();
  return nullptr;
}

auto GetElement::interpret() -> any {
  auto object = sub->interpret();
  auto index_ = index->interpret();
  if (isArray(object) && isNumber(index_))
    return getValueOfArray(object, index_);

  return nullptr;
}

auto SetElement::interpret() -> any {
  auto object = sub->interpret();
  auto index_ = index->interpret();
  auto value_ = value->interpret();
  if (isArray(object) && isNumber(index_))
    return setValueOfArray(object, index_, value_);

  return nullptr;
}

auto GetVariable::interpret() -> any {
  for (auto &variables : local.back()) {
    if (variables.count(name))
      return variables[name];
  }

  if (global.count(name))
    return global[name];

  if (functionTable.count(name))
    return functionTable[name];

  if (builtinFunctionTable.count(name))
    return builtinFunctionTable[name];

  return nullptr;
}

auto SetVariable::interpret() -> any {
  for (auto &variables : local.back()) {
    if (variables.count(name)) {
      return variables[name] = value->interpret();
    }
  }

  return global[name] = value->interpret();
}

auto NullLiteral::interpret() -> any { return nullptr; }

auto BooleanLiteral::interpret() -> any { return value; }

auto NumberLiteral::interpret() -> any { return value; }

auto StringLiteral::interpret() -> any { return value; }

auto ArrayLiteral::interpret() -> any {
  auto result = make_shared<vector<any>>();
  for (auto &node : values)
    result->push_back(node->interpret());
  return result;
}

auto MapLiteral::interpret() -> any { return nullptr; }
