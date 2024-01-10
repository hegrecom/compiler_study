#pragma once

#include "Node.h"
#include <any>
#include <ostream>

using std::any;
using std::ostream;
using std::string;

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

auto operator<<(ostream &stream, any &value) -> ostream &;
