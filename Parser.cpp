#include "Node.h"
#include "Token.h"
#include <iostream>

using std::cout;

static vector<Token>::iterator current;

auto parse(vector<Token> tokens) -> Program * {
  auto result = new Program();
  current = tokens.begin();
  while (current->kind != Kind::EndOfToken) {
    switch (current->kind) {
    default:
      cout << *current << "잘못된 구문입니다.";
      exit(1);
    }
  }

  return result;
}
