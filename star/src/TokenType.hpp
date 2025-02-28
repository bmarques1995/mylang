#pragma once

#include "DLLDef.hpp"

enum class STARLANG_API TokenType {
// Tokens de Caracteres Únicos
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET,
  RIGHT_BRACKET, COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, MUL_STAR,

  // Tokens de um ou dois Caracteres
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Tokens literais
  IDENTIFIER, STRING, NUMBER,

  // Palavras-chave
  AND, CLASS, ELSE, STAR_FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, STAR_TRUE, VAR, WHILE,

  // Token para final de declaração
  STAR_EOF 
};