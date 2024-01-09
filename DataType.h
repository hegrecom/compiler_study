#pragma once

#include <any>
#include <ostream>

using std::any;
using std::ostream;
using std::string;

auto isString(any value) -> bool;

auto isNumber(any value) -> bool;

auto toString(any value) -> string;

auto toNumber(any value) -> double;

auto operator<<(ostream &stream, any &value) -> ostream &;
