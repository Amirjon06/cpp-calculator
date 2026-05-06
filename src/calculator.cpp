#include "calculator.h"
#include <cctype>
#include <cmath>
#include <sstream>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Lexer
// ---------------------------------------------------------------------------

std::vector<Token> tokenize(const std::string& expr) {
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < expr.size()) {
        // Skip whitespace
        if (std::isspace(static_cast<unsigned char>(expr[i]))) {
            ++i;
            continue;
        }

        // Number (integer or floating-point)
        if (std::isdigit(static_cast<unsigned char>(expr[i])) || expr[i] == '.') {
            size_t start = i;
            bool hasDot = false;
            while (i < expr.size() &&
                   (std::isdigit(static_cast<unsigned char>(expr[i])) || expr[i] == '.')) {
                if (expr[i] == '.') {
                    if (hasDot)
                        throw CalcError("Invalid number: multiple decimal points near position " +
                                        std::to_string(i));
                    hasDot = true;
                }
                ++i;
            }
            double val = std::stod(expr.substr(start, i - start));
            tokens.push_back({TokenType::NUMBER, val});
            continue;
        }

        // Operators and parentheses
        switch (expr[i]) {
            case '+': tokens.push_back({TokenType::PLUS,    0}); break;
            case '-': tokens.push_back({TokenType::MINUS,   0}); break;
            case '*': tokens.push_back({TokenType::MULTIPLY,0}); break;
            case '/': tokens.push_back({TokenType::DIVIDE,  0}); break;
            case '(': tokens.push_back({TokenType::LPAREN,  0}); break;
            case ')': tokens.push_back({TokenType::RPAREN,  0}); break;
            default:
                throw CalcError(std::string("Unknown character '") + expr[i] +
                "' at position " + std::to_string(i));
        }
        ++i;
    }

    tokens.push_back({TokenType::END, 0});
    return tokens;
}

// ---------------------------------------------------------------------------
// Recursive-descent parser
// ---------------------------------------------------------------------------

namespace {

struct Parser {
    const std::vector<Token>& tokens;
    size_t pos = 0;

    const Token& peek() const { return tokens[pos]; }

    Token consume() {
        Token t = tokens[pos];
        if (t.type != TokenType::END) ++pos;
        return t;
    }

    Token expect(TokenType type) {
        Token t = consume();
        if (t.type != type)
            throw CalcError("Unexpected token at position " + std::to_string(pos - 1));
        return t;
    }

    // factor := NUMBER | '(' expr ')' | unary-minus factor
    double factor() {
        const Token& t = peek();

        if (t.type == TokenType::MINUS) {
            consume();
            return -factor();
        }
        if (t.type == TokenType::PLUS) {
            consume();
            return factor();
        }
        if (t.type == TokenType::NUMBER) {
            consume();
            return t.value;
        }
        if (t.type == TokenType::LPAREN) {
            consume();
            double val = expr();
            expect(TokenType::RPAREN);
            return val;
        }
        throw CalcError("Expected a number or '(' at token position " + std::to_string(pos));
    }

    // term := factor (('*' | '/') factor)*
    double term() {
        double result = factor();
        while (peek().type == TokenType::MULTIPLY || peek().type == TokenType::DIVIDE) {
            Token op = consume();
            double right = factor();
            if (op.type == TokenType::DIVIDE) {
                if (std::abs(right) < 1e-12)
                    throw CalcError("Division by zero");
                result /= right;
            } else {
                result *= right;
            }
        }
        return result;
    }

    // expr := term (('+' | '-') term)*
    double expr() {
        double result = term();
        while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
            Token op = consume();
            double right = term();
            if (op.type == TokenType::PLUS)
                result += right;
            else
                result -= right;
        }
        return result;
    }
};

}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

double evaluate(const std::vector<Token>& tokens) {
    Parser p{tokens};
    double result = p.expr();
    if (p.peek().type != TokenType::END)
        throw CalcError("Unexpected token after expression at position " +
        std::to_string(p.pos));
    return result;
}

double calculate(const std::string& expr) {
    return evaluate(tokenize(expr));
}