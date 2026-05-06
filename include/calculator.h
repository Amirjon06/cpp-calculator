#pragma once
#include <string>
#include <vector>
#include <stdexcept>

// Token types used during expression parsing
enum class TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType type;
    double value; // only used when type == NUMBER
};

// Thrown on malformed input (e.g. "3 + + 2", "1 / 0")
class CalcError : public std::runtime_error {
public:
    explicit CalcError(const std::string& msg) : std::runtime_error(msg) {}
};

// Lexer: converts a raw expression string into a flat list of tokens
std::vector<Token> tokenize(const std::string& expr);

// Parser / evaluator: evaluates a token stream respecting operator precedence
// Grammar:
//   expr   := term   (('+' | '-') term)*
//   term   := factor (('*' | '/') factor)*
//   factor := NUMBER | '(' expr ')' | '-' factor
double evaluate(const std::vector<Token>& tokens);

// Convenience wrapper: tokenize + evaluate in one call
double calculate(const std::string& expr);