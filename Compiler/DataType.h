#pragma once

#include "Node.h"
#include <any>
#include <functional>
#include <iomanip>
#include <memory>
#include <ostream>

using std::any;
using std::function;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;

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

auto toArray(any value) -> shared_ptr<vector<any>>;

auto getValueOfArray(any object, any index) -> any;

auto setValueOfArray(any object, any index, any value) -> any;

auto isMap(any value) -> bool;

auto toMap(any value) -> shared_ptr<map<string, any>>;

auto isSize(any value) -> bool;

auto toSize(any value) -> size_t;

auto getValueOfMap(any object, string key) -> any;

auto setValueOfMap(any object, string key, any value) -> any;

auto operator<<(ostream &stream, any &value) -> ostream &;
