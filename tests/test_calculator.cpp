#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "calculator.h"

// ---------------------------------------------------------------------------
// Basic arithmetic
// ---------------------------------------------------------------------------

TEST_CASE("addition") {
    CHECK(calculate("3 + 9") == doctest::Approx(12.0));
    CHECK(calculate("0 + 0") == doctest::Approx(0.0));
    CHECK(calculate("1.5 + 2.5") == doctest::Approx(4.0));
}

TEST_CASE("subtraction") {
    CHECK(calculate("10 - 4") == doctest::Approx(6.0));
    CHECK(calculate("0 - 5") == doctest::Approx(-5.0));
}

TEST_CASE("multiplication") {
    CHECK(calculate("6 * 7") == doctest::Approx(42.0));
    CHECK(calculate("3 * 0") == doctest::Approx(0.0));
}

TEST_CASE("division") {
    CHECK(calculate("9 / 3") == doctest::Approx(3.0));
    CHECK(calculate("1 / 4") == doctest::Approx(0.25));
}

// ---------------------------------------------------------------------------
// Operator precedence
// ---------------------------------------------------------------------------

TEST_CASE("operator precedence: * before +") {
    // 3 + 9 * 99 = 3 + 891 = 894  (not 1188)
    CHECK(calculate("3 + 9 * 99") == doctest::Approx(894.0));
}

TEST_CASE("operator precedence: / before -") {
    CHECK(calculate("20 - 10 / 2") == doctest::Approx(15.0));
}

TEST_CASE("operator precedence: left-to-right for equal precedence") {
    CHECK(calculate("8 / 4 / 2") == doctest::Approx(1.0));
    CHECK(calculate("5 - 3 + 1") == doctest::Approx(3.0));
}

// ---------------------------------------------------------------------------
// Parentheses
// ---------------------------------------------------------------------------

TEST_CASE("parentheses override precedence") {
    CHECK(calculate("(3 + 9) * 99") == doctest::Approx(1188.0));
    CHECK(calculate("(2 + 3) * (4 - 1)") == doctest::Approx(15.0));
}

TEST_CASE("nested parentheses") {
    CHECK(calculate("((2 + 3) * 2) + 1") == doctest::Approx(11.0));
}

// ---------------------------------------------------------------------------
// Unary minus
// ---------------------------------------------------------------------------

TEST_CASE("unary minus") {
    CHECK(calculate("-5 + 3") == doctest::Approx(-2.0));
    CHECK(calculate("10 * -2") == doctest::Approx(-20.0));
    CHECK(calculate("-(3 + 4)") == doctest::Approx(-7.0));
}

// ---------------------------------------------------------------------------
// Chained / multi-value expressions
// ---------------------------------------------------------------------------

TEST_CASE("chained operations") {
    CHECK(calculate("1 + 2 + 3 + 4 + 5") == doctest::Approx(15.0));
    CHECK(calculate("100 / 5 * 2 + 3 - 1") == doctest::Approx(42.0));
}

// ---------------------------------------------------------------------------
// Floating-point
// ---------------------------------------------------------------------------

TEST_CASE("floating-point numbers") {
    CHECK(calculate("1.1 + 2.2") == doctest::Approx(3.3));
    CHECK(calculate(".5 * 2") == doctest::Approx(1.0));
}

// ---------------------------------------------------------------------------
// Error handling
// ---------------------------------------------------------------------------

TEST_CASE("division by zero throws") {
    CHECK_THROWS_AS(calculate("1 / 0"), CalcError);
}

TEST_CASE("unknown character throws") {
    CHECK_THROWS_AS(calculate("3 $ 2"), CalcError);
}

TEST_CASE("missing operand throws") {
    CHECK_THROWS_AS(calculate("3 +"), CalcError);
    CHECK_THROWS_AS(calculate("* 3"), CalcError);
}

TEST_CASE("mismatched parentheses throws") {
    CHECK_THROWS_AS(calculate("(3 + 2"), CalcError);
    CHECK_THROWS_AS(calculate("3 + 2)"), CalcError);
}