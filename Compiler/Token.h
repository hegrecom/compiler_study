#pragma once

#include "Kind.h"
#include <string>

using namespace std;

struct Token {
  Kind kind = Kind::Unknown;
  string string;
};

auto printTokenList(vector<Token>) -> void;
auto operator<<(ostream &stream, Token &token) -> ostream &;
