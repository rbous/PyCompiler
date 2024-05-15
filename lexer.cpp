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
        char text;
        TokenType kind;

        // Constructor
        Token(char tokenText, TokenType tokenKind) {
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
            TokenType type;

            switch (curChar) 
            {
            case '+':
                type = TokenType::PLUS;
                break;
            case '-':
                type = TokenType::MINUS;
                break;
            case '*':
                type =  TokenType::ASTERISK;
                break;
            case '/':
                type =  TokenType::SLASH;
                break;
            case '\n':
                type = TokenType::NEWLINE;
                break;
            case '\0':
                type =  TokenType::ENDOFLINE;
                break;
            default:
                string msg = "Unknown token: ";
                msg += curChar;
                abort(msg);
            }

            Token token = Token(curChar, type);
            nextChar();
            return token;
        }
};




int main(void) {
    string source = "+- */";
    Lexer lexer = Lexer(source);

    Token token = lexer.getToken();
    while (token.kind != TokenType::ENDOFLINE) {
        cout << static_cast<std::underlying_type<TokenType>::type>(token.kind) << endl;
        token = lexer.getToken();
    }

}
