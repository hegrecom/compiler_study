#include "Kind.h"
#include "Node.h"
#include "Token.h"
#include <iostream>

using std::cout;

static vector<Token>::iterator current;

static auto parseFunction() -> Function *;
static auto parseBlock() -> vector<Statement *>;
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
  return *(new vector<Statement *>());
}
