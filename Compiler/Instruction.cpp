#include "Instruction.h"
#include <map>

using std::map;

static map<string, Instruction> stringToIntruction = {
    {"Exit", Instruction::Exit},

    {"Call", Instruction::Call},
    {"Alloca", Instruction::Alloca},
    {"Return", Instruction::Return},

    {"Jump", Instruction::Jump},
    {"ConditionJump", Instruction::ConditionJump},

    {"Print", Instruction::Print},
    {"PrintLine", Instruction::PrintLine},

    {"LogicalOr", Instruction::LogicalOr},
    {"LogicalAnd", Instruction::LogicalAnd},

    {"Equal", Instruction::Equal},
    {"NotEqual", Instruction::NotEqual},
    {"LessThan", Instruction::LessThan},
    {"GreaterThan", Instruction::GreaterThan},
    {"LessOrEqual", Instruction::LessOrEqual},
    {"GreaterOrEqual", Instruction::GreaterOrEqual},

    {"Add", Instruction::Add},
    {"Subtract", Instruction::Subtract},
    {"Multiply", Instruction::Multiply},
    {"Divide", Instruction::Divide},
    {"Modulo", Instruction::Modulo},

    {"Absolute", Instruction::Absolute},
    {"ReverseSign", Instruction::ReverseSign},

    {"GetElement", Instruction::GetElement},
    {"SetElement", Instruction::SetElement},
    {"GetGlobal", Instruction::GetGlobal},
    {"SetGlobal", Instruction::SetGlobal},
    {"GetLocal", Instruction::GetLocal},
    {"SetLocal", Instruction::SetLocal},

    {"PushNull", Instruction::PushNull},
    {"PushBoolean", Instruction::PushBoolean},
    {"PushNumber", Instruction::PushNumber},
    {"PushString", Instruction::PushString},
    {"PushArray", Instruction::PushArray},
    {"PushMap", Instruction::PushMap},
    {"PopOperand", Instruction::PopOperand},
};

static const auto instructionToString = [] {
  map<Instruction, string> result;
  for (auto &[key, value] : stringToIntruction)
    result[value] = key;
  return result;
}();

auto toString(Instruction instruction) -> string {
  if (instructionToString.count(instruction))
    return instructionToString.at(instruction);
  return "";
}
