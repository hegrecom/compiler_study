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

auto getValueOfArray(any object, any index) -> any {
  auto i = static_cast<int>(toNumber(index));
  if (i >= 0 && i < toArray(object)->size())
    return toArray(object)->at(i);

  return nullptr;
}

auto setValueOfArray(any object, any index, any value) -> any {
  auto i = static_cast<int>(toNumber(index));
  if (i >= 0 && i < toArray(object)->size())
    toArray(object)->at(i) = value;

  return value;
}

auto isMap(any value) -> bool {
  return value.type() == typeid(shared_ptr<map<string, any>>);
}

auto toMap(any value) -> shared_ptr<map<string, any>> {
  return any_cast<shared_ptr<map<string, any>>>(value);
}

auto getValueOfMap(any object, string key) -> any {
  if (toMap(object)->count(key))
    return toMap(object)->at(key);

  return nullptr;
}

auto setValueOfMap(any object, string key, any value) -> any {
  (*toMap(object))[key] = value;
  return value;
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
  if (isMap(value)) {
    stream << "{ ";
    auto temp = toMap(value);
    for (auto &element : *temp) {
      stream << element.first << ": " << element.second << " ";
    }
    stream << "}";
  }

  return stream;
}
