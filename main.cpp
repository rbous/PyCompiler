#include "lexer.hpp"

#include <fstream>

int main(void) {
    
    std::ifstream file("test.py");
    string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    string source = "if+async-123 foo_  *   then//\n    yep\r\n        lmao   \n    if True";
    Lexer lexer = Lexer(fileContent);

    Token token = lexer.getToken();
    while (token.kind != TokenType::ENDOFLINE) {
        std::cout << static_cast<std::underlying_type<TokenType>::type>(token.kind) << "\t" << token.text << std::endl;
        token = lexer.getToken();
    }
}
