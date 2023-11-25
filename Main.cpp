#include "Kind.h"
#include "Token.h"
#include <iomanip>
#include <iostream>
#include <map>

auto scan(string) -> vector<Token>;
auto printTokenList(vector<Token>) -> void;

auto main() -> int {
  string sourceCode = R""""(
        function main() {
            printLine 'Hello, World!'";
            printLine 1 + 2 * 3;
        }
    )"""";
  auto tokenList = scan(sourceCode);
  printTokenList(tokenList);

  return 0;
}
