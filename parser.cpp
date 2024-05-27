#include "parser.hpp"
#include "lexer.hpp"

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
