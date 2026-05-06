# C++ Calculator

A fully functional command-line calculator written in C++17. Parses and evaluates multi-value arithmetic expressions with correct operator precedence using a hand-written recursive-descent parser.

```
> 3 + 9 * 99
= 894

> (3 + 9) * 99
= 1188

> 100 / 5 * 2 + 3 - 1
= 42
```

---

## Features

- **Multi-value expressions** — chain as many numbers and operators as you like
- **Correct operator precedence** — `*` and `/` evaluate before `+` and `-`
- **Parentheses** — override precedence with `(` and `)`
- **Unary minus** — negative numbers and negated sub-expressions (`-5`, `-(3+4)`)
- **Floating-point support** — `1.5 * 2.0`, `.5 + .25`
- **Clear error messages** — division by zero, unknown characters, mismatched parentheses
- **Unit tests** — full test suite via [doctest](https://github.com/doctest/doctest)

---

## Project Structure

```
calculator/
├── include/
│   └── calculator.h        # Public API — tokenizer + evaluator declarations
├── src/
│   ├── calculator.cpp      # Lexer and recursive-descent parser implementation
│   └── main.cpp            # Interactive CLI entry point
├── tests/
│   └── test_calculator.cpp # Unit tests (doctest)
├── CMakeLists.txt
├── .gitignore
└── README.md
```

---

## How It Works

Expression evaluation happens in two stages:

**1. Lexer (`tokenize`)** — scans the input string character by character and produces a flat list of typed tokens: `NUMBER`, `PLUS`, `MINUS`, `MULTIPLY`, `DIVIDE`, `LPAREN`, `RPAREN`.

**2. Recursive-descent parser (`evaluate`)** — walks the token list using a grammar that encodes precedence directly in the call hierarchy:

```
expr   → term   (('+' | '-') term)*       ← lowest precedence
term   → factor (('*' | '/') factor)*
factor → NUMBER | '(' expr ')' | '-' factor  ← highest precedence
```

Because `term` is called inside `expr`, multiplication and division always bind tighter than addition and subtraction — no precedence tables or shunting-yard bookkeeping needed.

---

## Build

### Requirements

- CMake ≥ 3.15
- A C++17-compatible compiler (GCC ≥ 7, Clang ≥ 5, MSVC 2017+)
- Internet connection on first build (CMake fetches the doctest header automatically)

### Steps

```bash
# Clone
git clone https://github.com/<your-username>/calculator.git
cd calculator

# Configure
cmake -S . -B build

# Build
cmake --build build

# Run
./build/calculator
```

### Run tests

```bash
ctest --test-dir build --output-on-failure
```

Or run the test binary directly for verbose output:

```bash
./build/tests
```

---

## Usage

```
=== C++ Calculator ===
Type an expression and press Enter. Type 'help' or 'quit'.

> 3 + 9 * 99
= 894

> (2 + 3) * (4 - 1)
= 15

> 1 / 0
Error: Division by zero

> help

Calculator — supported operations
  +   addition          e.g.  3 + 5
  -   subtraction       e.g.  10 - 4
  *   multiplication    e.g.  6 * 7
  /   division          e.g.  9 / 3
  ()  parentheses       e.g.  (2 + 3) * 4
  Operator precedence is respected: * and / before + and -

Commands
  help   show this message
  quit   exit the program

> quit
Goodbye!
```

---

## License

MIT