#include "DataType.h"
#include <any>
#include <cmath>
#include <functional>
#include <map>
#include <string>
#include <vector>

using std::any;
using std::function;
using std::map;
using std::string;
using std::vector;

map<string, function<any(vector<any>)>> builtinFunctionTable = {
    {"sqrt",
     [](vector<any> values) -> any { return std::sqrt(toNumber(values[0])); }},
};
