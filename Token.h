#include "Kind.h"
#include <string>

using namespace std;

struct Token {
  Kind kind = Kind::Unkown;
  string string;
};
