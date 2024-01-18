#include "Code.h"
#include <iostream>

using std::any_cast;
using std::boolalpha;
using std::left;
using std::setw;

auto operator<<(ostream &stream, Code &code) -> ostream & {
  stream << setw(15) << left << toString(code.instruction);
  if (code.operand.type() == typeid(size_t)) {
    stream << "[" << any_cast<size_t>(code.operand) << "]";
  } else if (code.operand.type() == typeid(bool)) {
    stream << boolalpha << any_cast<bool>(code.operand);
  } else if (code.operand.type() == typeid(double)) {
    stream << any_cast<double>(code.operand);
  } else if (code.operand.type() == typeid(string)) {
    stream << "\"" << any_cast<string>(code.operand) << "\"";
  }
  return stream;
}
