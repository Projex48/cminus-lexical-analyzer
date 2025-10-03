/************************************************************/
// System includes

#include <iostream>

/************************************************************/
// Local includes

#include "Lexer.h"

/************************************************************/
// MACROS

#define BUFFER_SIZE 512

/************************************************************/
// Types

std::unordered_map<std::string, TokenType> keywordMap = {
  {"if", IF},     {"else", ELSE},     {"int", INT},
  {"void", VOID}, {"return", RETURN}, {"while", WHILE}};

/************************************************************/

Lexer::Lexer (FILE* sourceFile) : m_sourceFile (sourceFile)
{
}

Token
Lexer::getToken ()
{
  getChar ();

  // Get rid of unnecessary whitespace
  eatWhitespace ();

  eatComments ();

  switch (nextChar)
  {
    // Bookkeeping
    case EOF:
      return Token (END_OF_FILE);

    // Operators
    case '+':
      getChar ();
      if (nextChar != '+')
      {
        ungetChar ();
        return Token (PLUS, "+");
      }
      return Token (INCREMENT, "++");

    case '-':
      getChar ();
      if (nextChar != '-')
      {
        ungetChar ();
        return Token (MINUS, "-");
      }
      return Token (DECREMENT, "--");

    case '*':
      return Token (TIMES, "*");

    case '/':
      getChar ();
      ungetChar ();
      return Token (DIVIDE, "/");

    case '<':
      getChar ();
      if (nextChar != '=')
      {
        ungetChar ();
        return Token (LT, "<");
      }
      return Token (LTE, "<=");

    case '>':
      getChar ();
      if (nextChar != '=')
      {
        ungetChar ();
        return Token (GT, ">");
      }
      return Token (GTE, ">=");

    case '=':
      getChar ();
      if (nextChar != '=')
      {
        ungetChar ();
        return Token (ASSIGN, "=");
      }
      return Token (EQ, "==");

    case '!':
      getChar ();
      if (nextChar != '=')
      {
        ungetChar ();
        return Token (ERROR, "!");
      }
      return Token (NEQ, "!=");

    // Punctuators
    case ';':
      return Token (SEMI, ";");

    case ',':
      return Token (COMMA, ",");

    case '(':
      return Token (LPAREN, "(");

    case ')':
      return Token (RPAREN, ")");

    case '[':
      return Token (LBRACK, "[");

    case ']':
      return Token (RBRACK, "]");

    case '{':
      return Token (LBRACE, "{");

    case '}':
      return Token (RBRACE, "}");
  }

  if (isalpha (nextChar))
  {
    std::string str;
    str += nextChar;
    TokenType tokenType;
    while (isalpha (getChar ()))
      str += nextChar;
    ungetChar ();
    if (keywordMap.count (str) > 0)
    {
      tokenType = keywordMap.at (str);
      return Token (tokenType, str);
    }
    else
    {
      return Token (ID, str);
    }
  }

  if (isdigit (nextChar))
  {
    std::string str;
    str += nextChar;
    while (isdigit (getChar ()))
      str += nextChar;
    ungetChar ();
    return Token (NUM, str);
  }

  // If we got here, the char is invalid!
  std::string errorChar (1, nextChar);
  return Token (ERROR, errorChar);
};

int
Lexer::getLineNum () const
{
  return m_lineNum;
};

int
Lexer::getColumnNum () const
{
  return m_colNum + 1;
}

char
Lexer::getChar ()
{
  if (line.empty () || m_colNum >= line.length ())
  {
    char inLine[BUFFER_SIZE];
    if (fgets (inLine, BUFFER_SIZE, m_sourceFile))
    {
      line = std::string (inLine);
    }
    else
    {
      return nextChar = EOF;
    }
    ++m_lineNum;
    m_colNum = 0;
  }
  else
  {
    ++m_colNum;
  }
  nextChar = line[m_colNum];

  return nextChar;
}

void
Lexer::eatWhitespace ()
{
  while (nextChar == ' ' || nextChar == '\t' || nextChar == '\n' ||
         nextChar == '\0')
  {
    getChar ();
  }
};

void
Lexer::eatComments ()
{
  if (nextChar == '/')
  {
    getChar ();
    if (nextChar == '*')
    {
      bool inComment = true;
      while (inComment && nextChar != EOF) {
        if (getChar() == '*') {
          if (getChar() == '/') {
            getChar();
            break;
          }
        }
      } 
      eatWhitespace ();
    }
    else
    {
      ungetChar ();
    }
  }
}

char
Lexer::ungetChar ()
{
  --m_colNum;
  nextChar = line[m_colNum];
  return nextChar;
}

std::string
Lexer::tokenTypeToString (TokenType type)
{
  switch (type)
  {
    case END_OF_FILE:
      return "END_OF_FILE";
    case ERROR:
      return "ERROR";
    case IF:
      return "IF";
    case ELSE:
      return "ELSE";
    case INT:
      return "INT";
    case VOID:
      return "VOID";
    case RETURN:
      return "RETURN";
    case WHILE:
      return "WHILE";
    case PLUS:
      return "PLUS";
    case MINUS:
      return "MINUS";
    case TIMES:
      return "TIMES";
    case DIVIDE:
      return "DIVIDE";
    case LT:
      return "LT";
    case LTE:
      return "LTE";
    case GT:
      return "GT";
    case GTE:
      return "GTE";
    case EQ:
      return "EQ";
    case NEQ:
      return "NEQ";
    case ASSIGN:
      return "ASSIGN";
    case INCREMENT:
      return "INCREMENT";
    case DECREMENT:
      return "DECREMENT";
    case SEMI:
      return "SEMI";
    case COMMA:
      return "COMMA";
    case LPAREN:
      return "LPAREN";
    case RPAREN:
      return "RPAREN";
    case LBRACK:
      return "LBRACK";
    case RBRACK:
      return "RBRACK";
    case LBRACE:
      return "LBRACE";
    case RBRACE:
      return "RBRACE";
    case ID:
      return "ID";
    case NUM:
      return "NUM";
    default:
      return "UNKNOWN";
  }
}