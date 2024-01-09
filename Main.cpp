#include "Interpreter.h"
#include "Kind.h"
#include "Parser.h"
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
            printLine 1 + 2 * 3 * 8 / 4;
            printLine 99 % 8;

            var base2Number = 0b1010;
            var base8Number = 0o52;
            var base10Number = 42;
            var base16Number = 0x2afB13;

            var b = 8;
            printLine b;

            var a = b = 99;
            printLine a, b;

            if a == b {
              printLine 'a == b';
            } elif a != 8 {
              printLine 'a == 8';
            } elif b > 200 {
              printLine 'b > 200';
            } elif a <= 800 {
              printLine 'a <= 800';
            } else {
              printLine 'a != b';
            }

            var nullLiteral = null;

            for i = 0, i < 10, i = i + 1 {
              print i;
              for j = 10, j > 0, j = j - 1 {
                printLine j;
                continue;
              }
              if i > 5 {
                break;
              }
            }

            return 3 + 4;
        }
    )"""";
  auto tokenList = scan(sourceCode);
  printTokenList(tokenList);
  auto syntaxTree = parse(tokenList);
  printSyntaxTree(syntaxTree);

  interpret(syntaxTree);

  return 0;
}
