#include "lexer.hpp"
#include "parser.hpp"
#include <fstream>

using std::cout;
using std::endl;

int main(int argc, char **argv) {
    
    cout << "This is PyCompiler!\n-------------------" << endl;

    // Ensure only 2 arguments
    if (argc != 2) {
        cout << "Error: Compiler needs source file as argument." << endl;
        return 1;
    }

    // Get python file content
    std::ifstream file(argv[1]);
    string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Initialize the lexer and parser.
    Lexer lexer = Lexer(fileContent);
    Parser parser = Parser(lexer);

    // Print out every token
    Token token = lexer.getToken();
    while (token.kind != TokenType::ENDOFLINE) {
        std::cout << static_cast<std::underlying_type<TokenType>::type>(token.kind) << "\t" << token.text << std::endl;
        token = lexer.getToken();
    }
}
