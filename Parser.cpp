#include "Kind.h"
#include "Node.h"
#include "Token.h"
#include <iostream>
#include <set>

using std::cout;

static vector<Token>::iterator current;

static auto parseFunction() -> Function *;
static auto parseBlock() -> vector<Statement *>;
static auto parseVariable() -> Variable *;
static auto parseExpression() -> Expression *;
static auto parseExpressionStatement() -> ExpressionStatement *;
static auto parseReturn() -> Return *;
static auto parseFor() -> For *;
static auto parseBreak() -> Break *;
static auto parseContinue() -> Continue *;
static auto parseIf() -> If *;
static auto parsePrint() -> Print *;
static auto parseAssignment() -> Expression *;
static auto parseOr() -> Expression *;
static auto parseAnd() -> Expression *;
static auto parseRelational() -> Expression *;
static auto parseArithmetic1() -> Expression *;
static auto parseArithmetic2() -> Expression *;
static auto skipCurrent() -> void;
static auto skipCurrent(Kind kind) -> void;
static auto skipCurrentIf(Kind kind) -> bool;

auto parse(vector<Token> tokens) -> Program * {
  auto result = new Program();
  current = tokens.begin();
  while (current->kind != Kind::EndOfToken) {
    switch (current->kind) {
    case Kind::Function:
      result->functions.push_back(parseFunction());
      break;
    default:
      cout << *current << "잘못된 구문입니다.";
      exit(1);
    }
  }

  return result;
}

auto parseFunction() -> Function * {
  auto result = new Function();
  skipCurrent(Kind::Function);

  result->name = current->string;
  skipCurrent(Kind::Identifier);

  skipCurrent(Kind::LeftParen);
  if (current->kind != Kind::RightParen) {
    do {
      result->parameters.push_back(current->string);
      skipCurrent(Kind::Identifier);
    } while (skipCurrentIf(Kind::Comma));
  }
  skipCurrent(Kind::RightParen);
  skipCurrent(Kind::LeftBrace);
  result->block = parseBlock();
  skipCurrent(Kind::RightBrace);

  return result;
}

auto skipCurrent() -> void { current++; }

auto skipCurrent(Kind kind) -> void {
  if (current->kind != kind) {
    cout << toString(kind) + " 토큰이 필요합니다.";
    exit(1);
  }
  current++;
}

auto skipCurrentIf(Kind kind) -> bool {
  if (current->kind == kind) {
    current++;
    return true;
  } else {
    return false;
  }
}

auto parseBlock() -> vector<Statement *> {
  vector<Statement *> result;
  while (current->kind != Kind::RightBrace) {
    switch (current->kind) {
    case Kind::Variable:
      result.push_back(parseVariable());
      break;
    case Kind::Return:
      result.push_back(parseReturn());
      break;
    case Kind::For:
      result.push_back(parseFor());
      break;
    case Kind::Break:
      result.push_back(parseBreak());
      break;
    case Kind::Continue:
      result.push_back(parseContinue());
      break;
    case Kind::If:
      result.push_back(parseIf());
      break;
    case Kind::Print:
    case Kind::PrintLine:
      result.push_back(parsePrint());
      break;
    case Kind::EndOfToken:
      cout << *current << " 잘못된 구문입니다.";
      exit(1);
    default:
      result.push_back(parseExpressionStatement());
    }
  }

  return result;
}

auto parseVariable() -> Variable * {
  auto result = new Variable();
  skipCurrent(Kind::Variable);

  result->name = current->string;
  skipCurrent(Kind::Identifier);

  skipCurrent(Kind::Assignment);
  result->expression = parseExpression();

  skipCurrent(Kind::Semicolon);

  return result;
}

auto parseReturn() -> Return * {
  auto result = new Return();
  skipCurrent(Kind::Return);

  result->expression = parseExpression();
  if (result->expression == nullptr) {
    cout << "return 문에 식이 없습니다.";
    exit(1);
  }

  skipCurrent(Kind::Semicolon);

  return result;
}

auto parseExpressionStatement() -> ExpressionStatement * {
  auto result = new ExpressionStatement();
  result->expression = parseExpression();
  skipCurrent(Kind::Semicolon);

  return result;
}

auto parseFor() -> For * {
  auto result = new For();
  skipCurrent(Kind::For);

  result->variable = new Variable();
  result->variable->name = current->string;
  skipCurrent(Kind::Identifier);
  skipCurrent(Kind::Assignment);
  result->variable->expression = parseExpression();
  if (result->variable->expression == nullptr) {
    cout << "for 문의 초기화 식이 없습니다.";
    exit(1);
  }
  skipCurrent(Kind::Comma);
  result->condition = parseExpression();
  if (result->condition == nullptr) {
    cout << "for 문의 조건식이 없습니다.";
    exit(1);
  }
  skipCurrent(Kind::Comma);
  result->expression = parseExpression();
  if (result->expression == nullptr) {
    cout << "for 문의 증감식이 없습니다.";
    exit(1);
  }
  skipCurrent(Kind::LeftBrace);
  result->block = parseBlock();
  skipCurrent(Kind::RightBrace);

  return result;
}

auto parseBreak() -> Break * {
  auto result = new Break();
  skipCurrent(Kind::Break);
  skipCurrent(Kind::Semicolon);

  return result;
}

auto parseContinue() -> Continue * {
  auto result = new Continue();
  skipCurrent(Kind::Continue);
  skipCurrent(Kind::Semicolon);

  return result;
}

auto parseIf() -> If * {
  auto result = new If();
  skipCurrent(Kind::If);

  do {
    auto condition = parseExpression();
    if (condition == nullptr) {
      cout << "if 문의 조건식이 없습니다.";
      exit(1);
    }
    result->conditions.push_back(condition);
    skipCurrent(Kind::LeftBrace);
    result->blocks.push_back(parseBlock());
    skipCurrent(Kind::RightBrace);
  } while (skipCurrentIf(Kind::Elif));
  if (skipCurrentIf(Kind::Else)) {
    skipCurrent(Kind::LeftBrace);
    result->elseBlock = parseBlock();
    skipCurrent(Kind::RightBrace);
  }

  return result;
}

auto parsePrint() -> Print * {
  auto result = new Print();
  result->lineFeed = current->kind == Kind::PrintLine;
  skipCurrent();
  if (current->kind != Kind::Semicolon) {
    do
      result->arguments.push_back(parseExpression());
    while (skipCurrentIf(Kind::Comma));
  }

  return result;
}

auto parseExpression() -> Expression * {
  auto result = parseAssignment();

  return result;
}

auto parseAssignment() -> Expression * {
  auto result = parseOr();

  if (current->kind != Kind::Assignment)
    return result;
  skipCurrent(Kind::Assignment);

  if (auto getVariable = dynamic_cast<GetVariable *>(result)) {
    auto result = new SetVariable();
    result->name = getVariable->name;
    result->value = parseAssignment();
    return result;
  }

  if (auto getElement = dynamic_cast<GetElement *>(result)) {
    auto result = new SetElement();
    result->sub = getElement->sub;
    result->index = getElement->index;
    result->value = parseAssignment();
    return result;
  }

  cout << "잘못된 대입 연산 식입니다.";
  exit(1);
}

auto parseOr() -> Expression * {
  auto result = parseAnd();

  while (skipCurrentIf(Kind::LogicalOr)) {
    auto temp = new Or();
    temp->lhs = result;
    temp->rhs = parseAnd();
    result = temp;
  }

  return result;
}

auto parseAnd() -> Expression * {
  auto result = parseRelational();

  while (skipCurrentIf(Kind::LogicalAnd)) {
    auto temp = new And();
    temp->lhs = result;
    temp->rhs = parseRelational();
    result = temp;
  }

  return result;
}

auto parseRelational() -> Expression * {
  set<Kind> operators = {Kind::Equal,       Kind::NotEqual,
                         Kind::LessThan,    Kind::GreaterThan,
                         Kind::LessOrEqual, Kind::GreaterOrEqual};

  auto result = parseArithmetic1();
  while (operators.count(current->kind)) {
    auto temp = new Relational();
    temp->kind = current->kind;
    temp->lhs = result;
    temp->rhs = parseArithmetic1();
    result = temp;
  }

  return result;
}

auto parseArithmetic1() -> Expression * { return nullptr; }

auto parseArithmetic2() -> Expression * { return nullptr; }
