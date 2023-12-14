#include "Kind.h"
#include "Node.h"
#include "Token.h"
#include <iostream>

using std::cout;

static vector<Token>::iterator current;

static auto parseFunction() -> Function *;
static auto parseBlock() -> vector<Statement *>;
static auto parseVariable() -> Variable *;
static auto parseExpression() -> Expression *;
static auto parseReturn() -> Return *;
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
    case Kind::For:
    case Kind::Break:
    case Kind::Continue:
    case Kind::If:
    case Kind::Print:
    case Kind::PrintLine:
    case Kind::EndOfToken:
      cout << *current << " 잘못된 구문입니다.";
      exit(1);
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

auto parseExpression() -> Expression * { return nullptr; }
