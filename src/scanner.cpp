#include "scanner.h"

#include <iostream>
#include <unordered_map>
#include <utility>

#include "token.h"

Scanner::Scanner(std::string input) : input(std::move(input)), first(0), current(0) {}

bool Scanner::is_white_space(const char c) { return c == ' ' || c == '\r' || c == '\t'; }

bool Scanner::is_id_char(const char c) { return isalnum(c) || c == '_'; }

void Scanner::reset() {
  first = current = 0;
  line = column = 1;
}

static std::unordered_map<std::string, Token::Type> keywords = {
    {"var", Token::VAR},           {"val", Token::VAL},       {"fun", Token::FUN},
    {"class", Token::CLASS},       {"if", Token::IF},         {"else", Token::ELSE},
    {"while", Token::WHILE},       {"for", Token::FOR},       {"break", Token::BREAK},
    {"continue", Token::CONTINUE}, {"return", Token::RETURN}, {"in", Token::IN},
    {"this", Token::THIS},         {"Int", Token::INT_TYPE},  {"Boolean", Token::BOOL_TYPE},
    {"Unit", Token::UNIT_TYPE},    {"true", Token::TRUE},     {"false", Token::FALSE}};

Token *Scanner::nextToken() {
  // Skip whitespaces
  while (current < input.length() && is_white_space(input[current])) {
    current++;
    column++;
  }
  // Check if we reached the end of the input
  if (current >= input.length()) {
    return new Token(Token::ENDOFFILE, line, column);
  }

  const char c = input[current];

  // Handle newline after semicolon
  if (c == '\n') {
    current++;
    line++;
    column = 1;
    if (precededBySemicolon) {
      precededBySemicolon = false;
      return nextToken();
    }
    return new Token(Token::ENDL, "\\n", 0, 2, line, column);
  }

  first = current;
  const int tokenLine = line, tokenColumn = column;

  // Number literals
  if (isdigit(c)) {
    current++;
    column++;
    while (current < input.length() && isdigit(input[current])) {
      current++;
      column++;
    }
    precededBySemicolon = false;
    return new Token(Token::INT_LITERAL, input, first, current - first, tokenLine, tokenColumn);
  }

  // Identifier or keyword
  if (isalpha(c)) {
    current++;
    column++;
    while (current < input.length() && is_id_char(input[current])) {
      current++;
      column++;
    }
    const std::string word = input.substr(first, current - first);
    const Token::Type type = keywords.contains(word) ? keywords[word] : Token::ID;
    precededBySemicolon = false;
    return new Token(type, word, 0, word.length(), tokenLine, tokenColumn);
  }

  // Two character tokens
  if (current + 1 < input.length()) {
    const std::string two = input.substr(current, 2);
    if (two == "==") {
      current += 2;
      column += 2;
      precededBySemicolon = false;
      return new Token(Token::EQEQ, tokenLine, tokenColumn);
    }
    if (two == "!=") {
      current += 2;
      column += 2;
      precededBySemicolon = false;
      return new Token(Token::NEQ, tokenLine, tokenColumn);
    }
    if (two == "<=") {
      current += 2;
      column += 2;
      precededBySemicolon = false;
      return new Token(Token::LTE, tokenLine, tokenColumn);
    }
    if (two == ">=") {
      current += 2;
      column += 2;
      precededBySemicolon = false;
      return new Token(Token::GTE, tokenLine, tokenColumn);
    }
    if (two == "&&") {
      current += 2;
      column += 2;
      precededBySemicolon = false;
      return new Token(Token::AND, tokenLine, tokenColumn);
    }
    if (two == "||") {
      current += 2;
      column += 2;
      precededBySemicolon = false;
      return new Token(Token::OR, tokenLine, tokenColumn);
    }
    if (two == "..") {
      current += 2;
      column += 2;
      precededBySemicolon = false;
      return new Token(Token::RANGE, tokenLine, tokenColumn);
    }
  }

  // Single character tokens
  Token::Type type;
  switch (c) {
    case '+':
      type = Token::PLUS;
      break;
    case '-':
      type = Token::MINUS;
      break;
    case '*':
      type = Token::MUL;
      break;
    case '/':
      type = Token::DIV;
      break;
    case '=':
      type = Token::ASSIGN;
      break;
    case '<':
      type = Token::LT;
      break;
    case '>':
      type = Token::GT;
      break;
    case '!':
      type = Token::NOT;
      break;
    case '(':
      type = Token::LPAREN;
      break;
    case ')':
      type = Token::RPAREN;
      break;
    case '{':
      type = Token::LBRACE;
      break;
    case '}':
      type = Token::RBRACE;
      break;
    case ':':
      type = Token::COLON;
      break;
    case ',':
      type = Token::COMMA;
      break;
    case '.':
      type = Token::DOT;
      break;
    case ';':
      type = Token::SEMICOLON;
      precededBySemicolon = true;
      break;
    default:
      current++;
      column++;
      precededBySemicolon = false;
      return new Token(Token::ERROR, c, tokenLine, tokenColumn);
  }

  current++;
  column++;
  return new Token(type, c, tokenLine, tokenColumn);
}

void Scanner::test() {
  Token *current;
  std::cout << "Testing Scanner:\n\n";
  while ((current = nextToken())->type != Token::ENDOFFILE) {
    if (current->type == Token::ERROR) {
      std::cout << "Scanner error, invalid character:\n" << current;
      delete current;
      return;
    }
    std::cout << current;
    delete current;
  }
  std::cout << current;
  delete current;
}
