#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::vector;

// Enum class for Token Types
enum class TokenType {

    INDENT = -4,
    DEDENT = -3,

    SYNTAXERROR = -2,

    ENDOFLINE = -1,
    NEWLINE = 0,
    NUMBER = 1,
    IDENT = 2,
    STRING = 3,

    INT = 4,
    STR = 5,
    BOOL = 6,

    // Keywords
    FALSE = 101,
    NONE = 102,
    TRUE = 103,
    AND = 104,
    AS = 105,
    CLASS = 110,
    DEF = 112,
    ELIF = 114,
    ELSE = 115,
    FOR = 118,
    GLOBAL = 120,
    IF = 121,
    IN = 123,
    IS = 124,
    NONLOCAL = 126,
    NOT = 127,
    OR = 128,
    PASS = 129,
    RETURN = 131,
    WHILE = 133,

    // Operators  
    EQ = 201, 
    PLUS = 202,
    MINUS = 203,
    ASTERISK = 204,
    DBLSLASH = 205,
    EQEQ = 206,
    NOTEQ = 207,
    LT = 208,
    LTEQ = 209,
    GT = 210,
    GTEQ = 211,
    LEFTPRNTH = 212,
    RIGHTPRNTH = 213,
    LEFTBRCKT = 214,
    RIGHTBRCKT = 215,
    COMMA = 216,
    COLON = 217,
    DOT = 218,
    ARROW = 219,
    MODULUS = 220,

};

// Token class
class Token {
public:
    string text;
    TokenType kind;
    Token(string tokenText, TokenType tokenKind);
};

// Lexer class
class Lexer {
public:
    char curChar;
    int curPos;
    string source;
    vector<int> indentStack;
    bool newLine;

    Lexer(string s);
    void nextChar();
    char peek();
    void abort(string message);
    void skipWhitespace();
    void skipComment();
    static TokenType checkIfKeyword(string text);
    int calculateIndentation();
    Token handleIndents();
    Token getToken();
};

#endif // LEXER_H