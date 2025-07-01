#include <fstream>
#include <iostream>
#include <string>
#include "parser/parser.h"
#include "scanner/scanner.h"
#include "visitor/visitor.h"

int main(const int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect amount of arguments. Used: " << argv[0] << " <input_file>" << '\n';
        exit(1);
    }
    std::fstream infile(argv[1]);
    if (!infile.is_open()) {
        std::cerr << "Error opening file " << argv[1] << '\n';
        exit(1);
    }

    std::string input;
    std::string line;
    while (std::getline(infile, line)) {
        input += line + "\n";
    }
    infile.close();

    Scanner scanner(input);
    // scanner.test();
    // scanner.reset();
    Parser parser(&scanner);
    cout << "\nStarting parsing:\n";
    try {
        Program* program = parser.parseProgram();
        cout << "Parsing successful\n\n";
        // PrintVisitor print_visitor;
        // cout << "Starting PrintVisitor:\n\n";
        // print_visitor.imprimir(program);

        ofstream outfile("output.s");
        if (!outfile.is_open()) {
            cerr << "Error al crear el archivo de salida: output.s" << endl;
            return 1;
        }

        GenCodeVisitor codigo(outfile);
        cout << "\n\nStarting code generation:\n";
        codigo.generar(program);
        outfile.close();
        cout << "Code generation successful. Output written to output.s\n";
        delete program;
    } catch (const exception& e) {
        cout << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }
    return 0;
}
