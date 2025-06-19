#include <fstream>
#include <iostream>
#include <string>

#include "parser.h"
#include "scanner.h"
#include "visitor.h"

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
    scanner.test();
    scanner.reset();
    cout << "Scanner exitoso" << endl;
    cout << endl;
    cout << "Iniciando parsing:" << endl;
    Parser parser(&scanner);
    try {
        Program* program = parser.parseProgram();
        cout << "Parsing exitoso" << endl << endl;
        cout << "Iniciando Visitor:" << endl;
        PrintVisitor print_visitor;
        cout << "Iniciando PrintVisitor:" << endl;
        print_visitor.imprimir(program);

        ofstream outfile("output.s");
        if (!outfile.is_open()) {
            cerr << "Error al crear el archivo de salida: output.s" << endl;
            return 1;
        };

        // cout << "Generando codigo ensamblador en " << outputFilename << endl;

        // GenCodeVisitor codigo(outfile);
        // codigo.generar(program);
        // outfile.close();
        delete program;
    } catch (const exception& e) {
        cout << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }

    return 0;
}
