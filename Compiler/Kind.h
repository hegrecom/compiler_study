#pragma once

#include <string>

using std::string;

struct Token;

enum class Kind {
  Unknown,
  EndOfToken,
  NullLiteral,
  TrueLiteral,
  FalseLiteral,
  NumberLiteral,
  StringLiteral,
  Identifier,

  Function,
  Return,
  Variable,
  For,
  Break,
  Continue,
  If,
  Elif,
  Else,
  Print,
  PrintLine,

  LogicalAnd,
  LogicalOr,
  Assignment,
  Add,
  Subtract,
  Multiply,
  Divide,
  Modulo,
  Equal,
  NotEqual,
  LessThan,
  GreaterThan,
  LessOrEqual,
  GreaterOrEqual,

  Comma,
  Colon,
  Semicolon,
  LeftParen,
  RightParen,
  LeftBrace,
  RightBrace,
  LeftBracket,
  RightBracket,
};

auto toKind(string) -> Kind;
auto toString(Kind type) -> string;
