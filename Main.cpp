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
            printLine 'Hello, World!';
            printLine 1 + 2 * 3;

            var base2Number = 0b1010;
            var base8Number = 0o52;
            var base10Number = 42;
            var base16Number = 0x2afB13;
        }
    )"""";
  auto tokenList = scan(sourceCode);
  printTokenList(tokenList);

  return 0;
}
