#include <criterion/criterion.h>
#include <SilikegoCore/StringSource.h>
#include <SilikegoCore/Lexer.h>

using Silikego::TokenStatus;
using Silikego::EndOfInput;

#define TestLexer(NAME, INSTRING, ...) \
Test(LexerTests, NAME) \
{ \
    std::string input(INSTRING); \
    std::unique_ptr<Silikego::DataSource> source = \
        std::make_unique<Silikego::StringSource>(input); \
    std::vector<Silikego::Token> tokens \
    { \
        __VA_ARGS__ \
    }; \
    Silikego::Lexer lex(std::move(source)); \
    for (int i = 0; i < tokens.size(); i++) \
    { \
        Silikego::Token current = lex.current(); \
        cr_assert(current.status() == tokens[i].status()); \
        switch(current.status()) \
        { \
        case TokenStatus::Operator: \
            cr_assert(current.operatorValue() == tokens[i].operatorValue()); \
            break; \
        case TokenStatus::Integer: \
            cr_assert(current.integerValue() == tokens[i].integerValue()); \
            break; \
        case TokenStatus::Float: \
            cr_assert(current.floatValue() == tokens[i].floatValue()); \
            break; \
        default: \
            break; \
        } \
        lex.advance(); \
    } \
}

TestLexer(Nothing,
    "",
    EndOfInput()
)

TestLexer(AnInteger,
    "42386",
    42386,
    EndOfInput()
)

TestLexer(AFloat,
    "32156.25",
    32156.25,
    EndOfInput()

)

TestLexer(AnId,
    "beep",
    std::string("beep"),
    EndOfInput()
)

TestLexer(TwoPlusTwo,
    "2 + 2.0",
    2,
    '+',
    2.0,
    EndOfInput()
)

TestLexer(SomethingComplex,
    "3d6 + -5 * (sin(3.25)/cos(5.125))",
    3,
    'd',
    6,
    '+',
    '-',
    5,
    '*',
    '(',
    std::string("sin"),
    '(',
    3.25,
    ')',
    '/',
    std::string("cos"),
    '(',
    5.125,
    ')',
    ')',
    EndOfInput()
)

// No, this won't parse using the existing parser, but you could write
// one yourself if you wanted to.
TestLexer(SomethingComplexLispy,
    "(*\n"
    "\t(+ (d 3 6) -5)\n"
    "\t(/ (sin 3.25)\n"
    "\t\t(cos 5.125)))\n",
    '(',
    '*',
    '(',
    '+',
    '(',
    'd',
    3,
    6,
    ')',
    '-',
    5,
    ')',
    '(',
    '/',
    '(',
    std::string("sin"),
    3.25,
    ')',
    '(',
    std::string("cos"),
    5.125,
    ')',
    ')',
    ')',
    EndOfInput()
)
