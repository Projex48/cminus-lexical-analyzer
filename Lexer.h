#ifndef LEXER_H
#define LEXER_H

/***********************************************************************/
// System includes

#include <fstream>
#include <string>
#include <unordered_map>

/***********************************************************************/
// Enums

enum TokenType
{
  // Bookkeeping
  END_OF_FILE,
  ERROR,

  // Keywords
  IF,
  ELSE,
  INT,
  VOID,
  RETURN,
  WHILE,

  // Operators
  PLUS,
  MINUS,
  TIMES,
  DIVIDE,
  LT,
  LTE,
  GT,
  GTE,
  EQ,
  NEQ,
  ASSIGN,
  INCREMENT,
  DECREMENT,

  // Punctuators
  SEMI,
  COMMA,
  LPAREN,
  RPAREN,
  LBRACK,
  RBRACK,
  LBRACE,
  RBRACE,

  // Identifiers and integer literals
  ID,
  NUM
};

/***********************************************************************/
// Types

struct Token
{
  Token (TokenType pType, std::string pLexeme = "", int pNumber = 0)
    : type (pType), lexeme (pLexeme), number (pNumber)
  {
  }

  TokenType type;
  std::string lexeme;
  int number;
};

/***********************************************************************/

class Lexer
{
public:
  Lexer (FILE* sourceFile);

  Token
  getToken ();

  int
  getLineNum () const;

  int
  getColumnNum () const;

  std::string
  tokenTypeToString (TokenType type);

private:
  char
  getChar ();

  void
  eatWhitespace ();

  void
  eatComments ();

  char
  ungetChar ();

private:
  FILE* m_sourceFile;
  int m_nextChar;
  int m_lineNum = 0;
  size_t m_colNum;
  char nextChar;

  std::string line;

  // Additional data members if necessary
  // ...
};

/***********************************************************************/

#endif