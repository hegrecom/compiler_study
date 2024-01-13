#pragma once

#include "Code.h"
#include "Node.h"
#include <cstddef>
#include <map>
#include <tuple>
#include <vector>

using std::map;
using std::size_t;
using std::tuple;
using std::vector;

auto generate(Program *) -> tuple<vector<Code>, map<string, size_t>>;
