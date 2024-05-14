#include <iostream>
#include <stdio.h>

using std::string;
using std::cout;
using std::endl;

class Lexer {
    public:
        char curChar;
        int curPos;
        string source;

    public:
        // Contructor
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
        void abort(void) {
        }

        // Skip whitespace except newlines
        void skipWhitespace(void) {
        }

        // Skip comments in the code
        void skipComment(void) {
        }

        // Return the next token
        void getToken(void) {
        }
};

int main(void) {
    string source = "LET foobar = 123";
    Lexer lexer = Lexer(source);

    while (lexer.peek() != '\0') {
        cout << lexer.curChar << endl;
        lexer.nextChar();
    }

}
