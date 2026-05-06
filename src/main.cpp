#include "calculator.h"
#include <iostream>
#include <string>
#include <iomanip>

static void printHelp() {
    std::cout << "\nCalculator — supported operations\n"
              << "  +   addition          e.g.  3 + 5\n"
              << "  -   subtraction       e.g.  10 - 4\n"
              << "  *   multiplication    e.g.  6 * 7\n"
              << "  /   division          e.g.  9 / 3\n"
              << "  ()  parentheses       e.g.  (2 + 3) * 4\n"
              << "  Operator precedence is respected: * and / before + and -\n\n"
              << "Commands\n"
              << "  help   show this message\n"
              << "  quit   exit the program\n\n";
}

int main() {
    std::cout << "=== C++ Calculator ===\n";
    std::cout << "Type an expression and press Enter. Type 'help' or 'quit'.\n\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break; // EOF (Ctrl+D / Ctrl+Z)

        // Trim leading/trailing whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end   = line.find_last_not_of(" \t\r\n");
        if (start == std::string::npos) continue; // blank line
        line = line.substr(start, end - start + 1);

        if (line == "quit" || line == "exit" || line == "q") break;
        if (line == "help" || line == "h" || line == "?") { printHelp(); continue; }

        try {
            double result = calculate(line);

            // Print as integer when the result has no fractional part
            if (result == static_cast<long long>(result) &&
                result >= -1e15 && result <= 1e15) {
                std::cout << "= " << static_cast<long long>(result) << "\n\n";
            } else {
                std::cout << "= " << std::setprecision(10) << result << "\n\n";
            }
        } catch (const CalcError& e) {
            std::cerr << "Error: " << e.what() << "\n\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}