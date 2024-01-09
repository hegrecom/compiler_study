#include "DataType.h"
#include <iomanip>

using std::any_cast;

auto isString(any value) -> bool { return value.type() == typeid(string); }

auto isNumber(any value) -> bool { return value.type() == typeid(double); }

auto toString(any value) -> string { return any_cast<string>(value); }

auto toNumber(any value) -> double { return any_cast<double>(value); }

auto operator<<(ostream &stream, any &value) -> ostream & {
  if (isString(value))
    stream << toString(value);
  if (isNumber(value))
    stream << toNumber(value);

  return stream;
}
