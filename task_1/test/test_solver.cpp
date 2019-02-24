#include <gtest/gtest.h>
#include <main.h>

TEST(test_has_variable, test_solver)
{
    std::string input = "8 - 10 * x + 3";
    Parser p(input);
    auto ast = p.parse();
    auto result = Solver::hasVariable(ast);
    ASSERT_EQ(result, true);
}

TEST(test_has_not_variable, test_solver)
{
    std::string input = "8 - 10 + 3";
    Parser p(input);
    auto ast = p.parse();
    auto result = Solver::hasVariable(ast);
    ASSERT_EQ(result, false);
}

TEST(test_simplify_expr_without_variable, test_solver)
{
    std::string input = "8 - 10 + 3";
    Parser p(input);
    auto ast = p.parse();
    auto result = Solver::simplify(ast);
    ASSERT_EQ(result, true);
    auto number = dynamic_cast<Number*>(ast.get());
    ASSERT_NE(number, nullptr);
    ASSERT_EQ(number->value, 1);
}

TEST(test_solve_X_eq_7_plus_3_minus_1, test_solver)
{
    std::string input = "x = 7 + 3 - 1";
    Parser p(input);
    auto ast = p.parse();
    int result;
    auto solved = Solver::solve(ast, result);
    ASSERT_EQ(solved, true);
    ASSERT_EQ(result, 9);
}

TEST(test_solve_8_minus_5_mult_X_plus_6_div_2_eq_9_minus_8, test_solver)
{
    std::string input = "8 - 5*x + 6/2 = 9 - 8";
    Parser p(input);
    auto ast = p.parse();
    int result;
    auto solved = Solver::solve(ast, result);
    ASSERT_EQ(solved, true);
    ASSERT_EQ(result, 2);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
