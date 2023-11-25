#include "Kind.h"
#include <string>

using namespace std;

struct Token {
  Kind kind = Kind::Unknown;
  string string;
};
