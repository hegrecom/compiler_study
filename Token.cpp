#include "Token.h"
#include "Kind.h"
#include <iomanip>
#include <iostream>

using std::cout;

auto operator<<(ostream &stream, Token &token) -> ostream & {
  return stream << setw(12) << left << toString(token.kind) << token.string;
}

auto printTokenList(vector<Token> tokenList) -> void {
  cout << setw(12) << left << "KIND"
       << "STRING" << endl;
  cout << string(23, '-') << endl;
  for (auto &token : tokenList)
    cout << token << endl;
}
