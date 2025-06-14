#include <fstream>
#include <iostream>

#include "scanner.h"

int main(const int argc, const char *argv[]) {
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
  return 0;
}
