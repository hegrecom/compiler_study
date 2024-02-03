#pragma once

#include "Code.h"
#include <map>
#include <tuple>
#include <vector>

using std::tuple;
using std::vector;

auto execute(tuple<vector<Code>, map<string, size_t>>) -> void;
