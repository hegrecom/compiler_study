#pragma once

#include "DataType.h"
#include "Instruction.h"
#include <any>
#include <iomanip>
#include <ios>
#include <ostream>

using std::any;
using std::ostream;
using std::tuple;

struct Code {
  Instruction instruction;
  any operand;
};

auto operator<<(ostream &stream, Code &code) -> ostream &;

auto printObjectCode(tuple<vector<Code>, map<string, size_t>> objectCode)
    -> void;
