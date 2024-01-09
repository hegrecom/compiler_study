#include "DataType.h"
#include <iomanip>

using std::any_cast;

auto isString(any value) -> bool { return value.type() == typeid(string); }

auto toString(any value) -> string { return any_cast<string>(value); }

auto operator<<(ostream &stream, any &value) -> ostream & {
  if (isString(value))
    stream << toString(value);
  else
    stream << "Printing the following data types are not supported: "
           << value.type().name();

  return stream;
}
