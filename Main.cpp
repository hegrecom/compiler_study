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
            printLine true and false;
            printLine false and true;
            printLine true and true;
            printLine true or true;
            printLine false or true;
            printLine false or false;

            global = 4;
            var local = 13;
            printLine 'global = ', global;
            printLine 'local = ', local;
            global = local = 8;
            printLine 'updated global = ', global;
            printLine 'updated local = ', local;

            var base2Number = 0b1010;
            var base8Number = 0o52;
            var base10Number = 42;
            var base16Number = 0x2afB13;

            var b = 8;
            printLine b;

            var a = b = 99;
            printLine a, ', ', b;

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
              printLine 'i: ', i;
              for j = 10, j > 0, j = j - 1 {
                if j == 3 {
                  continue;
                }
                printLine j;
              }
              if i > 5 {
                break;
              }
            }

            for i = 0, i < 5, i = i + 1 {
              if i == 1 {
                printLine 'one';
              } elif i == 2 {
                printLine 'two';
              } elif i == 3 {
                printLine 'three';
              } else {
                printLine i;
              }
            }
            haHaHa();
            printLine 'add 99 + 1: ', add(99, 1);
            printLine sqrt(16);
            printLine [1, 2, 4];

            var arr = ['first', 'second', 'third'];
            printLine arr[1];
            arr[1] = 'updated second';
            printLine arr[1];

            return 3 + 4;
        }

        function haHaHa() {
          printLine 'HaHaHa';
        }

        function add(a, b) {
          return a + b;
        }
    )"""";
  auto tokenList = scan(sourceCode);
  printTokenList(tokenList);
  auto syntaxTree = parse(tokenList);
  printSyntaxTree(syntaxTree);

  interpret(syntaxTree);

  return 0;
}
