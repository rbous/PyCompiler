#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"

class Parser {
public:
    Lexer lexer;
    Token curToken;
    Token peekToken;

    Parser(Lexer lxr);
    bool checkToken(TokenType kind);
    bool checkPeek(TokenType kind);
    void match(TokenType kind);
    void nextToken();
    void abort(string message);

    void program();
    void statement();
    void nl();
 //   void expression();
};

#endif // PARSER_H