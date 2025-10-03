/************************************************************/
// System includes

#include <cstdio>
#include <fstream>
#include <iostream>

/************************************************************/
// Local includes

#include "Lexer.h"
#include <vector>

/************************************************************/
// Variables

int errorBufferIndex = 0;

/************************************************************/
// Constants

const int COL_OFFSET = 20;

/************************************************************/

int
main (int argc, char* argv[])
{
  std::string filename;
  FILE* inFile;
  FILE* outFile;

  if (argc < 2)
  {
    inFile = stdin;
    outFile = stdout;
  }
  else
  {
    if (argc == 4 && std::string (argv[2]) == "-o")
    {
      outFile = fopen (argv[3], "w+");
    }
    else
    {
      outFile = fopen ("a.out", "w+");
    }

    inFile = fopen (argv[1], "r");
  }

  Lexer lexer = Lexer (inFile);
  auto scOut = [&outFile] (std::string s) { fputs (s.c_str (), outFile); };
  std::vector<Token> tokenBuffer;
  std::vector<std::vector<int>> errorBuffer;
  Token token = lexer.getToken ();
  while (token.type != END_OF_FILE)
  {
    tokenBuffer.push_back (token);
    if (token.type == ERROR)
      errorBuffer.push_back ({lexer.getLineNum (), lexer.getColumnNum ()});
    token = lexer.getToken ();
  }
  scOut ("TOKEN               LEXEME              VALUE               \n");
  scOut ("=====               ======              =====               \n");
  for (size_t i = 0; i < tokenBuffer.size (); ++i)
  {
    token = tokenBuffer[i];
    std::string tokType = lexer.tokenTypeToString (token.type);
    std::string lexeme = token.lexeme;
    int col2Offset = COL_OFFSET - tokType.length ();
    int col3Offset = COL_OFFSET - lexeme.length () - 2;
    scOut (tokType);
    for (int i = 0; i < col2Offset; ++i)
      scOut (" ");
    scOut ("\"" + lexeme + "\"");
    for (int i = 0; i < col3Offset; ++i)
      scOut (" ");
    if (token.type == ERROR)
    {
      std::string out =
        "Line: " + std::to_string (errorBuffer[errorBufferIndex][0]) +
        "; Column: " + std::to_string (errorBuffer[errorBufferIndex][1]);
      scOut (out);
      ++errorBufferIndex;
    }
    if (token.type == ID)
      scOut ("\"" + lexeme + "\"");
    if (token.type == NUM)
      scOut (lexeme);
    scOut ("\n");
    token = lexer.getToken ();
  }
  scOut ("END_OF_FILE         \"\" ");
}