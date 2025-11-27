#include <criterion/criterion.h>
#include <SilikegoCore/StringSource.h>
#include <SilikegoCore/Lexer.h>

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
        Silikego::Token current = lex.GetToken(); \
        cr_assert(current.Type() == tokens[i].Type(), \
            "Types[%i]: %i/%c %i/%c", \
            i, \
            current.Type(), \
            current.Type(), \
            tokens[i].Type(), \
            tokens[i].Type() \
        ); \
        switch(current.Type()) \
        { \
        case Silikego::Token::TokenType::INTEGER: \
            cr_assert(current.Integer() == tokens[i].Integer()); \
            break; \
        case Silikego::Token::TokenType::FLOAT: \
            cr_assert(current.Float() == tokens[i].Float()); \
            break; \
        default: \
            break; \
        } \
        lex.Next(); \
    } \
}

TestLexer(Nothing,
    "",
    Silikego::Token::TokenType::EOL
)

TestLexer(AnInteger,
    "42386",
    42386
)

TestLexer(AFloat,
    "32156.25",
    32156.25
)

TestLexer(AnId,
    "beep",
    std::string("beep")
)

TestLexer(TwoPlusTwo,
    "2 + 2.0",
    2,
    Silikego::Token::TokenType::ADDITION,
    2.0,
    Silikego::Token::TokenType::EOL
)

TestLexer(SomethingComplex,
    "3d6 + -5 * (sin(3.25)/cos(5.125))",
    3,
    Silikego::Token::TokenType::DICE,
    6,
    Silikego::Token::TokenType::ADDITION,
    Silikego::Token::TokenType::SUBTRACT,
    5,
    Silikego::Token::TokenType::MULTIPLY,
    Silikego::Token::TokenType::LPAREN,
    std::string("sin"),
    Silikego::Token::TokenType::LPAREN,
    3.25,
    Silikego::Token::TokenType::RPAREN,
    Silikego::Token::TokenType::DIVISION,
    std::string("cos"),
    Silikego::Token::TokenType::LPAREN,
    5.125,
    Silikego::Token::TokenType::RPAREN,
    Silikego::Token::TokenType::RPAREN,
    Silikego::Token::TokenType::EOL,
)

// No, this won't parse using the existing parser, but you could write
// one yourself if you wanted to.
TestLexer(SomethingComplexLispy,
    "(*\n"
    "\t(+ (d 3 6) -5)\n"
    "\t(/ (sin 3.25)\n"
    "\t\t(cos 5.125)))\n",
    Silikego::Token::TokenType::LPAREN,
    Silikego::Token::TokenType::MULTIPLY,
    Silikego::Token::TokenType::LPAREN,
    Silikego::Token::TokenType::ADDITION,
    Silikego::Token::TokenType::LPAREN,
    Silikego::Token::TokenType::DICE,
    3,
    6,
    Silikego::Token::TokenType::RPAREN,
    Silikego::Token::TokenType::SUBTRACT,
    5,
    Silikego::Token::TokenType::RPAREN,
    Silikego::Token::TokenType::LPAREN,
    Silikego::Token::TokenType::DIVISION,
    Silikego::Token::TokenType::LPAREN,
    std::string("sin"),
    3.25,
    Silikego::Token::TokenType::RPAREN,
    Silikego::Token::TokenType::LPAREN,
    std::string("cos"),
    5.125,
    Silikego::Token::TokenType::RPAREN,
    Silikego::Token::TokenType::RPAREN,
    Silikego::Token::TokenType::RPAREN,
    Silikego::Token::TokenType::EOL,
)
