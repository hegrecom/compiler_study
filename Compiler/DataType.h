#pragma once

#include "Node.h"
#include <any>
#include <functional>
#include <iomanip>
#include <ostream>

using std::any;
using std::function;
using std::ostream;
using std::string;
using std::vector;

struct Object {
  bool isMarked = false;
  virtual ~Object() {}
};

struct Array : Object {
  vector<any> values;
};

struct Map : Object {
  map<string, any> values;
};

auto isString(any value) -> bool;

auto toString(any value) -> string;

auto isNumber(any value) -> bool;

auto toNumber(any value) -> double;

auto isBoolean(any value) -> bool;

auto toBoolean(any value) -> bool;

auto isTrue(any value) -> bool;

auto isFalse(any value) -> bool;

auto isFunction(any value) -> bool;

auto toFunction(any value) -> Function *;

auto isBuiltinFunction(any value) -> bool;

auto toBuiltinFunction(any value) -> function<any(vector<any>)>;

auto isArray(any value) -> bool;

auto toArray(any value) -> Array *;

auto getValueOfArray(any object, any index) -> any;

auto setValueOfArray(any object, any index, any value) -> any;

auto isMap(any value) -> bool;

auto toMap(any value) -> Map *;

auto isSize(any value) -> bool;

auto toSize(any value) -> size_t;

auto getValueOfMap(any object, string key) -> any;

auto setValueOfMap(any object, string key, any value) -> any;

auto operator<<(ostream &stream, any &value) -> ostream &;
