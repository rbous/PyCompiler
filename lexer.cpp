#include <iostream>
#include <string>
#include <unordered_map>

using std::string;
using std::cout;
using std::endl;

enum class TokenType {

    INDENT = -4,
    DEDENT = -3,
    
    SYNTAXERROR = -2,

    ENDOFLINE = -1,
    NEWLINE = 0,
	NUMBER = 1,
	IDENT = 2,
	STRING = 3,

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
	SLASH = 205,
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
    DBLSLASH = 221,

};

std::unordered_map<string, TokenType> keywords = {
    {"False", TokenType::FALSE},
    {"True", TokenType::TRUE},
    {"None", TokenType::NONE},
    {"and", TokenType::AND},
    {"as", TokenType::AS},
    {"class", TokenType::CLASS},
    {"def", TokenType::DEF},
    {"elif", TokenType::ELIF},
    {"else", TokenType::ELSE},
    {"for", TokenType::FOR},
    {"global", TokenType::GLOBAL},
    {"if", TokenType::IF},
    {"in", TokenType::IN},
    {"is", TokenType::IS},
    {"nonlocal", TokenType::NONLOCAL},
    {"not", TokenType::NOT},
    {"or", TokenType::OR},
    {"pass", TokenType::PASS},
    {"return", TokenType::RETURN},
    {"while", TokenType::WHILE},

    // Illegal identifiers
    {"assert", TokenType::SYNTAXERROR},
    {"async", TokenType::SYNTAXERROR},
    {"await", TokenType::SYNTAXERROR},
    {"break", TokenType::SYNTAXERROR},
    {"continue", TokenType::SYNTAXERROR},
    {"del", TokenType::SYNTAXERROR},
    {"except", TokenType::SYNTAXERROR},
    {"finally", TokenType::SYNTAXERROR},
    {"from", TokenType::SYNTAXERROR},
    {"import", TokenType::SYNTAXERROR},
    {"lambda", TokenType::SYNTAXERROR},
    {"raise", TokenType::SYNTAXERROR},
    {"try", TokenType::SYNTAXERROR},
    {"with", TokenType::SYNTAXERROR},
    {"yield", TokenType::SYNTAXERROR},
};


class Token {
    public:
        string text;
        TokenType kind;

        // Constructor
        Token(string tokenText, TokenType tokenKind) {
            text = tokenText;
            kind = tokenKind;
        };
};

class Lexer {
    public:
        char curChar;
        int curPos;
        string source;

    public:
        // Constructor
        Lexer(string s) {
            source = s + "\n";
            curChar = ' ';
            curPos = -1;
            nextChar();
        };

        // Process the next character
        void nextChar(void) {
            curPos++;
            if (curPos >= source.length()) {
                curChar = '\0';
            } else {
                curChar = source[curPos];
            }
        }
        
        // Return the lookahead character
        char peek(void) {
            if (curPos + 1 >= source.length()) {
                return '\0';
            } 
            return source[curPos + 1];
        }

        // Invalid token found, print error message and exit
        void abort(string message) {
            cout << "Lexing error. " << message << endl;
            exit(1);
        }

        // Skip whitespace except newlines
        void skipWhitespace(void) {
            while (curChar == ' ' || curChar == '\t' || curChar == '\r') {
                nextChar();
            }
        }

        // Skip comments in the code
        void skipComment(void) {
            if (curChar == '#') {
                while (curChar != '\n') {
                    nextChar();
                }
            }
        }

        static TokenType checkIfKeyword(string text) {
            auto it = keywords.find(text);
            if (it != keywords.end())
                return it->second;
            return TokenType::IDENT;
        }

        // Return the next token
        Token getToken(void) {
            skipWhitespace();
            skipComment();

            TokenType type;
            string curString = "";
            curString += curChar;
            int startPos = curPos;

            switch (curChar) 
            {
            case '+':
                type = TokenType::PLUS;
                break;
            case '-':
                type = TokenType::MINUS;
                break;
            case '*':
                type = TokenType::ASTERISK;
                break;
            case '/':
                type = TokenType::SLASH;
                break;
            case '\n':
                type = TokenType::NEWLINE;
                break;
            case '\0':
                type = TokenType::ENDOFLINE;
                break;
            case '=':
                // check if token is = or ==
                if (peek() == '=') {
                    nextChar();
                    curString += curChar;
                    type = TokenType::EQEQ;
                } else {
                    type = TokenType::EQ;
                }
                break;
            case '>':
                // check if token is > or >=
                if (peek() == '=') {
                    nextChar();
                    curString += curChar;
                    type = TokenType::GTEQ;
                } else {
                    type = TokenType::GT;
                }
                break;
            case '<':
                // check if token is > or >=
                if (peek() == '=') {
                    nextChar();
                    curString += curChar;
                    type = TokenType::LTEQ;
                } else {
                    type = TokenType::LT;
                }
                break;
            case '!':
                if (peek() == '=') {
                    nextChar();
                    curString += curChar;
                    type = TokenType::NOTEQ;
                } else {
                    string msg = "Expected !=, got !";
                    msg += peek();
                    abort(msg);
                }
                break;
            case '\"':
                // Get characters between quotations
                nextChar();
                
                while (curChar != '\"') {
                    // No escape characters, newlines, tabs, or % in the string (because of C's printf)
                    if (curChar == '\r'|| curChar == '\n' || curChar == '\t' || curChar == '\\' || curChar == '%') {
                        abort("Illegal character in string.");
                    }
                    nextChar();
                }
                curString = source.substr(startPos, curPos - startPos + 1);
                type = TokenType::STRING;
                break;


            default:

                // Leading character is a digit, so this must be a number.
                // Get all consecutive digits and decimal if there is one.
                if (isdigit(curChar)) {

                    startPos = curPos;
                    while (isdigit(peek())) {
                        nextChar();
                    }
                    if (peek() == '.') { // decimal
                        nextChar();
                        // Must have at least one digit after decimal.
                        if (!isdigit(peek())) {
                            abort("Illegal character in number.");
                        }
                        while (isdigit(peek())) {
                            nextChar();
                        }
                    }
                    curString = source.substr(startPos, curPos - startPos + 1);
                    type = TokenType::NUMBER;


                // Leading character is a letter, so this must be an identifier or a keyword.
                // Get all consecutive alpha numeric characters.
                } else if (isalpha(curChar)) {

                    startPos = curPos;
                    while (isalnum(peek())) {
                        nextChar();
                    }

                    // Check if the token is in the list of keywords.
                    curString = source.substr(startPos, curPos - startPos + 1);
                    type = Lexer::checkIfKeyword(curString);



                } else {
                    string msg = "Unknown token: ";
                    msg += curChar;
                    abort(msg);
                }
            }

            Token token = Token(curString, type);
            nextChar();
            return token;
        }
};




int main(void) {
    string source = "+-123if \"bomboclaat\"9.8654*/";
    Lexer lexer = Lexer(source);

    Token token = lexer.getToken();
    while (token.kind != TokenType::ENDOFLINE) {
        cout << static_cast<std::underlying_type<TokenType>::type>(token.kind) << "\t" << token.text << endl;
        token = lexer.getToken();
    }

}
