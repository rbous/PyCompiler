#include "lexer.hpp"

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


// Token class constructor
Token::Token(string tokenText, TokenType tokenKind) {
    text = tokenText;
    kind = tokenKind;
}


// Lexer class constructor
Lexer::Lexer(string s) {
    source = s + "\n";
    curChar = ' ';
    curPos = -1;
    indentStack.push_back(0);
    newLine = true;
    nextChar();
}

// Process the next character
void Lexer::nextChar() {
    curPos++;
    if (curPos >= source.length()) {
        curChar = '\0';
    } else {
        curChar = source[curPos];
    }
}
        
// Return the lookahead character
char Lexer::peek() {
    if (curPos + 1 >= source.length()) {
        return '\0';
    } 
    return source[curPos + 1];
}

// Invalid token found, print error message and exit
void Lexer::abort(string message) {
    std::cout << "Lexing error. " << message << std::endl;
    exit(1);
}

// Skip whitespace except newlines
void Lexer::skipWhitespace() {
    while (curChar == ' ' || curChar == '\t' || curChar == '\r') {
        nextChar();
    }
}

// Skip comments in the code
void Lexer::skipComment() {
    if (curChar == '#') {
        while (curChar != '\n') {
            nextChar();
        }
    }
}

TokenType Lexer::checkIfKeyword(string text) {
    auto it = keywords.find(text);
    if (it != keywords.end())
        return it->second;
    return TokenType::IDENT;
}

// Calculate the current indentation level
int Lexer::calculateIndentation() {
    int count;
    for (count = 0; curChar == ' '; count++) {
        nextChar();
    }
    return count;
}

// Handle indents and dedents
Token Lexer::handleIndents() {
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
            abort("Indentation error.");
        }
        return Token("DEDENT", TokenType::DEDENT);
    }
    // None
    return getToken();
}

// Return the next token
Token Lexer::getToken() {

    if (newLine) {
        newLine = false;
        int newPos = curPos;
        skipWhitespace();

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
            while (isdigit(peek())) {
                nextChar();
                curString += curChar;
            }
            if (peek() == '.') {
                abort("Floating point numbers are not supported in ChocoPy");
            }
            type = TokenType::NUMBER;

        // Leading character is a letter, so this must be an identifier or a keyword.
        // Get all consecutive alpha numeric characters.
        } else if (isalpha(curChar) || curChar == '_') {
            while (isalnum(peek()) || peek() == '_') {
                nextChar();
                curString += curChar;
            }
            // Check if the token is in the list of keywords.
            type = Lexer::checkIfKeyword(curString);

        } else {
            string msg = "Unknown token: ";
            msg += curChar;
            abort(msg);
        }
    }

    nextChar();
    return Token(curString, type);
}

/* TODO: 
 *
 *  types (int, str, etc)
 *  print, input, len
 * 
 */
