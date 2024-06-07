#include "parser.hpp"
#include "lexer.hpp"

using std::cout;
using std::endl;

// Parser object keeps track of current token and checks if the code matches the grammar.

// Constructor
Parser::Parser(Lexer lxr) {
    lexer = lxr;
    curToken = Token();
    peekToken = Token();
    nextToken();
    nextToken(); // Call this twice to initialize current and peek.
}

// Return true if the current token matches.
bool Parser::checkToken(TokenType kind) {
    return kind == curToken.kind;
}

// Return true if the next token matches.
bool Parser::checkPeek(TokenType kind) {
    return kind == peekToken.kind;
}

// Try to match current token. If not, error. Advances the current token.
void Parser::match(TokenType kind) {
    if (!checkToken(kind)) {
        string msg = "Expected ";
        msg += static_cast<std::underlying_type<TokenType>::type>(kind);
        msg += ", got ";
        msg += static_cast<std::underlying_type<TokenType>::type>(curToken.kind);
        abort(msg);
    }
}

// Advances the current token.
void Parser::nextToken() {
    curToken = peekToken;
    peekToken = lexer.getToken();
    // No need to worry about passing the EOF, lexer handles that.
}

// Print error message and exit
void Parser::abort(string message) {
    std::cout << "Error. " << message << std::endl;
    exit(2);
}


// Production rules

// program ::= {statement}
void Parser::program() {
    cout << "PROGRAM" << endl;

    // Parse all the statements in the program.
    while (!checkToken(TokenType::ENDOFLINE)) {
        statement();
    }
}

// statement ::= "print" (expression | string) nl
void Parser::statement() {
    // Check the first token to see what kind of statement this is.

    // "print" (expression | string)
    if (checkToken(TokenType::PRINT)) {
        cout << "STATEMENT-PRINT" << endl;
        nextToken();

        if (checkToken(TokenType::STRING)) {
            // Simple string.
            nextToken();
        } else {
            // Expect an expression
            //expression();
        }
    }
    // Newline
    nl();
}

// nl ::= '\n'+
void Parser::nl() {
    cout << "NEWLINE" << endl;

    // Require at least one newline.
    match(TokenType::NEWLINE);
    // Allow extra newlines too.
    while (checkToken(TokenType::NEWLINE)) {
        nextToken();
    }
}