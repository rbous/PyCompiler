#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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
        std::vector<int> indentStack;
        bool newLine;

    public:
        // Constructor
        Lexer(string s) {
            source = s + "\n";
            curChar = ' ';
            curPos = -1;
            indentStack.push_back(0);
            newLine = true;
            nextChar();
        };

        // Process the next character
        void nextChar() {
            curPos++;
            if (curPos >= source.length()) {
                curChar = '\0';
            } else {
                curChar = source[curPos];
            }
        }
        
        // Return the lookahead character
        char peek() {
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
        void skipWhitespace() {
            while (curChar == ' ' || curChar == '\t' || curChar == '\r') {
                nextChar();
            }
        }

        // Skip comments in the code
        void skipComment() {
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

        // Calculate the current indentation level
        int calculateIndentation() {
            int count;
            for (count = 0; curChar == ' '; count++) {
                nextChar();
            }
            return count;
        }

        // Handle indents and dedents
        Token handleIndents() {
            int curIndent = calculateIndentation();
            // Indent
            if (curIndent > indentStack.back()) {
                indentStack.push_back(curIndent);
                return Token("INDENT", TokenType::INDENT);
            } 
            // Dedent (may contain nested loops/if statements)
            while (curIndent < indentStack.back()) {
                indentStack.pop_back();
                if (curIndent > indentStack.back()) {
                    cout << curIndent << " " << indentStack.back() << endl;
                    abort("Indentation error.");
                }
                return Token("DEDENT", TokenType::DEDENT);
            }
            // None
            return getToken();
        }

        // Return the next token
        Token getToken(void) {
            if (newLine) {
                newLine = false;
                int newPos = curPos;

                // Skip blank lines or lines with comments
                while (curChar == '\n' || curChar == '\r') {
                    nextChar();
                    newPos = curPos;
                    skipWhitespace();
                    skipComment();
                }
                // Go back to beginning of line to correctly calculate indentation
                curPos = newPos;
                curChar = source[curPos];

                // If it's still a newline, handle indents
                if (curChar != '\n' && curChar != '\0') {
                    return handleIndents();
                }
            }

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
            case '*':
                type = TokenType::ASTERISK;
                break;
            case '\n':
                newLine = true;
                type = TokenType::NEWLINE;
                break;
            case '\0':
                type = TokenType::ENDOFLINE;
                break;
            case '%':
                type = TokenType::MODULUS;
                break;
            case '(':
                type = TokenType::LEFTPRNTH;
                break;
            case ')':
                type = TokenType::RIGHTPRNTH;
                break;
            case '[':
                type = TokenType::LEFTBRCKT;
                break;
            case ']':
                type = TokenType::RIGHTBRCKT;
                break;
            case '.':
                type = TokenType::DOT;
                break;
            case ':':
                type = TokenType::COLON;
                break;
            case ',':
                type = TokenType::COMMA;
                break;
            case '-':
                // check if token is - or ->
                if (peek() == '>') {
                    nextChar();
                    curString += curChar;
                    type = TokenType::ARROW;
                } else {
                    type = TokenType::MINUS;
                }
                break;
            case '/':
                // check if token is / or invalid
                if (peek() == '/') {
                    nextChar();
                    curString += curChar;
                    type = TokenType::DBLSLASH;
                } else {
                    string msg = "Expected //, got /";
                    msg += peek();
                    abort(msg);
                }
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
                // check if token is < or <=
                if (peek() == '=') {
                    nextChar();
                    curString += curChar;
                    type = TokenType::LTEQ;
                } else {
                    type = TokenType::LT;
                }
                break;
            case '!':
                // check if token is != or invalid
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
                        abort("Floating point numbers are not supported in ChocoPy");
                    }
                    curString = source.substr(startPos, curPos - startPos + 1);
                    type = TokenType::NUMBER;


                // Leading character is a letter, so this must be an identifier or a keyword.
                // Get all consecutive alpha numeric characters.
                } else if (isalpha(curChar) || curChar == '_') {

                    startPos = curPos;
                    while (isalnum(peek()) || peek() == '_') {
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

/* TODO: 
 *
 *  indents/dedents
 *  types (int, str, etc)
 *  print, input, len
 * 
 */

#include <fstream>

int main(void) {
    
    std::ifstream file("test.py");
    string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    string source = "if+async-123 foo_  *   then//\n    yep\r\n        lmao   \n    if True";
    Lexer lexer = Lexer(fileContent);

    Token token = lexer.getToken();
    while (token.kind != TokenType::ENDOFLINE) {
        cout << static_cast<std::underlying_type<TokenType>::type>(token.kind) << "\t" << token.text << endl;
        token = lexer.getToken();
    }
}
