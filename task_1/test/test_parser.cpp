#include <gtest/gtest.h>
#include <main.h>

TEST(test_parse_number, test_parser)
{
    std::string input = "456";
    Parser parser(input);
    auto expr = parser.parse();
    auto number = dynamic_cast<Number*>(expr.get());
    ASSERT_NE(number, nullptr);
}

TEST(test_parse_number_value, test_parser)
{
    std::string input = "456";
    Parser parser(input);
    auto expr = parser.parse();
    auto number = dynamic_cast<Number*>(expr.get());
    ASSERT_EQ(number->value, 456);
}

TEST(test_parse_variable, test_parser)
{
    std::string input = "x";
    Parser parser(input);
    auto expr = parser.parse();
    auto var = dynamic_cast<Variable*>(expr.get());
    ASSERT_NE(var, nullptr);
    ASSERT_EQ(var->name, input);
}

TEST(test_parse_12_plus_34, test_parser)
{
    std::string input = "12 + 34";
    Parser p(input);
    auto expr = p.parse();
    auto addition = dynamic_cast<Operation*>(expr.get());
    ASSERT_NE(addition, nullptr);
    auto lhs = dynamic_cast<Number*>(addition->lhs.get());
    ASSERT_NE(lhs, nullptr);
    ASSERT_EQ(lhs->value, 12);
    auto rhs = dynamic_cast<Number*>(addition->rhs.get());
    ASSERT_NE(rhs, nullptr);
    ASSERT_EQ(rhs->value, 34);
}

TEST(test_parse_12_plus_34_minus_7, test_parser)
{
    std::string input = "12 + 34 - 7";
    Parser p(input);
    auto expr = p.parse();
    // Тестируем +
    auto addition = dynamic_cast<Operation*>(expr.get());
    ASSERT_NE(addition, nullptr);
    ASSERT_EQ(addition->operation, '+');
    // Тестируем 12
    auto lhs = dynamic_cast<Number*>(addition->lhs.get());
    ASSERT_NE(lhs, nullptr);
    ASSERT_EQ(lhs->value, 12);
    // Тестируем -
    auto subtraction = dynamic_cast<Operation*>(addition->rhs.get());
    ASSERT_NE(subtraction, nullptr);
    ASSERT_EQ(subtraction->operation, '-');
    // Тестируем 34
    auto subtraction_lhs = dynamic_cast<Number*>(subtraction->lhs.get());
    ASSERT_NE(subtraction_lhs, nullptr);
    ASSERT_EQ(subtraction_lhs->value, 34);
    // Тестируем 7
    auto subtraction_rhs = dynamic_cast<Number*>(subtraction->rhs.get());
    ASSERT_NE(subtraction_rhs, nullptr);
    ASSERT_EQ(subtraction_rhs->value, 7);
}



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
