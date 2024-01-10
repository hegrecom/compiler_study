#include "DataType.h"
#include <vector>

using std::any_cast;

auto isString(any value) -> bool { return value.type() == typeid(string); }

auto toString(any value) -> string { return any_cast<string>(value); }

auto isNumber(any value) -> bool { return value.type() == typeid(double); }

auto toNumber(any value) -> double { return any_cast<double>(value); }

auto isBoolean(any value) -> bool { return value.type() == typeid(bool); }

auto toBoolean(any value) -> bool { return any_cast<bool>(value); }

auto isTrue(any value) -> bool { return isBoolean(value) && toBoolean(value); }

auto isFalse(any value) -> bool {
  return isBoolean(value) && !toBoolean(value);
}

auto isFunction(any value) -> bool {
  return value.type() == typeid(Function *);
}

auto toFunction(any value) -> Function * { return any_cast<Function *>(value); }

auto isBuiltinFunction(any value) -> bool {
  return value.type() == typeid(function<any(vector<any>)>);
};

auto toBuiltinFunction(any value) -> function<any(vector<any>)> {
  return any_cast<function<any(vector<any>)>>(value);
}

auto isArray(any value) -> bool {
  return value.type() == typeid(shared_ptr<vector<any>>);
}

auto toArray(any value) -> shared_ptr<vector<any>> {
  return any_cast<shared_ptr<vector<any>>>(value);
}

auto operator<<(ostream &stream, any &value) -> ostream & {
  if (isString(value))
    stream << toString(value);
  if (isNumber(value))
    stream << toNumber(value);
  if (isBoolean(value)) {
    if (isTrue(value))
      stream << "true";
    else
      stream << "false";
  }
  if (isArray(value)) {
    stream << "[ ";
    auto temp = toArray(value);
    for (auto &element : *temp) {
      stream << element << " ";
    }
    stream << "]";
  }

  return stream;
}
