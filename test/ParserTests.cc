#include <criterion/criterion.h>
#include <SilikegoCore/InfixParser.h>
#include <SilikegoCore/StringInput.h>
#include <SilikegoCore/Node.h>
#include <SilikegoCore/Evaluate.h>

#include <memory>
#include <string>

using std::unique_ptr;

using Silikego::Input;
using Silikego::StringInput;
using Silikego::FunctionCaller;
using Silikego::Node;
using Silikego::Value;
using Silikego::ValueStatus;

#define ParserTest(NAME, INPUT, TARGET) \
Test(ParserTests, NAME) \
{ \
	std::string In = INPUT; \
	std::unique_ptr<Input> src = std::make_unique<StringInput>(In); \
	cr_assert(src != NULL); \
	FunctionCaller caller; \
    InstallOperators(caller); \
    InstallFunctions(caller); \
\
	Node tree = Silikego::ParseInfix(std::move(src)); \
    Value target{TARGET}; \
	Value result{Silikego::Evaluate(tree, caller)}; \
    cr_assert(target.status() == result.status()); \
    switch(result.status()) \
    { \
    case ValueStatus::Integer: \
        cr_assert(target.integer() == result.integer()); \
        break; \
    case ValueStatus::Real: \
        cr_assert(target.real() == result.real()); \
        break; \
    default: \
        break; \
    } \
}

ParserTest(TwoPlusThreeIsfive, "2 + 3", 5)
ParserTest(TwoPlusThreeIsfiveWithFloats, "2.0 + 3.0", 5.0)
ParserTest(FunctionAndParens, "sin(0.0) + 4", 4.0)
ParserTest(LostOfTimes, "multiply(2, 3, 5)", 30)
