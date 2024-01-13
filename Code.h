#include "Instruction.h"
#include <any>

using std::any;

struct Code {
  Instruction instruction;
  any operand;
}
