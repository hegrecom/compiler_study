#include "Token.h"
#include <iostream>

static string::iterator current;

enum class CharType {
  Unknown,
  WhiteSpace,
  NumberLiteral,
  StringLiteral,
  IdentifierAndKeyword,
  OperatorAndPunctuator,
};

auto getCharType(char) -> CharType;
auto scanNumberLiteral() -> Token;
auto scanStringLiteral() -> Token;
auto scanIdentifierAndKeyword() -> Token;
auto scanOperatorAndPunctuator() -> Token;
auto isCharType(char, CharType) -> bool;

auto scan(string sourceCode) -> vector<Token> {
  vector<Token> result;
  sourceCode += '\0';
  current = sourceCode.begin();
  while (*current != '\0') {
    switch (getCharType(*current)) {
    case CharType::WhiteSpace:
      current += 1;
      break;
    case CharType::NumberLiteral:
      result.push_back(scanNumberLiteral());
      break;
    case CharType::StringLiteral:
      result.push_back(scanStringLiteral());
      break;
    case CharType::IdentifierAndKeyword:
      result.push_back(scanIdentifierAndKeyword());
      break;
    case CharType::OperatorAndPunctuator:
      result.push_back(scanOperatorAndPunctuator());
      break;
    default:
      cout << *current << "사용할 수 없는 문자입니다.";
      exit(1);
    }
  }
  result.push_back({Kind::EndOfToken});
  return result;
}

auto getCharType(char c) -> CharType {
  if (' ' == c || '\t' == c || '\n' == c || '\r' == c)
    return CharType::WhiteSpace;
  if ('0' <= c && c <= '9')
    return CharType::NumberLiteral;
  if ('\'' == c)
    return CharType::StringLiteral;
  if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_')
    return CharType::IdentifierAndKeyword;
  if (33 <= c && c <= 47 && c != '\'' || 58 <= c && c <= 64 ||
      91 <= c && c <= 96 && c != '_' || 123 <= c && c <= 126)
    return CharType::OperatorAndPunctuator;

  return CharType::Unknown;
}

auto scanNumberLiteral() -> Token {
  string string;
  while (isCharType(*current, CharType::NumberLiteral))
    string += *current++;
  if (*current == '.') {
    string += *current++;
    while (isCharType(*current, CharType::NumberLiteral))
      string += *current++;
  }

  return Token{Kind::NumberLiteral, string};
}

auto scanStringLiteral() -> Token {
  string string;
  current++;
  while (isCharType(*current, CharType::StringLiteral))
    string += *current++;
  if (*current != '\'') {
    cout << "문자열의 종료 문자가 없습니다.";
    exit(1);
  }
  current++;

  return Token{Kind::StringLiteral, string};
}

auto isCharType(char c, CharType type) -> bool {
  switch (type) {
  case CharType::NumberLiteral:
    return '0' <= c && c <= '9';
  case CharType::StringLiteral:
    return 32 <= c && c <= 126 && c != '\'';
  default:
    return false;
  }
}
