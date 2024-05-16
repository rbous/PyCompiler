#include <iostream>

using std::string;
using std::cout;
using std::endl;

enum class TokenType {
    ENDOFLINE = -1,
    NEWLINE = 0,
	NUMBER = 1,
	IDENT = 2,
	STRING = 3,
    // Keywords
    LABEL = 101,
	GOTO = 102,
	PRINT = 103,
	INPUT = 104,
	LET = 105,
	IF = 106,
	THEN = 107,
	ENDIF = 108,
	WHILE = 109,
	REPEAT = 110,
	ENDWHILE = 111,
    // Operators (ADD OTHERS)
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

        // Return the next token
        Token getToken(void) {
            skipWhitespace();
            skipComment();

            TokenType type;
            string curString = "";
            curString += curChar;
            int startPos = curPos;
            int charCount = 1;

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
                    charCount++;
                }
                curString = source.substr(startPos, charCount + 1);
                type = TokenType::STRING;
                break;


            default:

                // Leading character is a digit, so this must be a number.
                // Get all consecutive digits and decimal if there is one.
                if (isdigit(curChar))
                {
                    startPos = curPos;
                    while (isdigit(peek())) {
                        nextChar();
                        charCount++;
                    }
                    if (peek() == '.') { // decimal
                        nextChar();
                        charCount++;
                        // Must have at least one digit after decimal.
                        if (!isdigit(peek())) {
                            abort("Illegal character in number.");
                        }
                        while (isdigit(peek())) {
                            nextChar();
                            charCount++;
                        }
                    }
                    curString = source.substr(startPos, charCount);
                    type = TokenType::NUMBER;
                    
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
    string source = "+-123 9.8654*/";
    Lexer lexer = Lexer(source);

    Token token = lexer.getToken();
    while (token.kind != TokenType::ENDOFLINE) {
        cout << static_cast<std::underlying_type<TokenType>::type>(token.kind) << endl;
        token = lexer.getToken();
    }

}
