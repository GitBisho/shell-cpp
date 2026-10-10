#include <iostream>
#include <string>

#include "tokenizer.h"

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        tokenize_input(line); 
        if (line.empty()) continue;
    }
}
