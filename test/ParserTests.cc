#include <criterion/criterion.h>
#include <SilikegoCore/InfixParser.h>
#include <SilikegoCore/StringSource.h>
#include <SilikegoCore/SyntaxTree.h>

#include <memory>
#include <string>

using std::unique_ptr;

using Silikego::DataSource;
using Silikego::StringSource;
using Silikego::FunctionCaller;
using Silikego::SyntaxTreeNode;
using Silikego::Value;
using Silikego::ValueStatus;

#define ParserTest(NAME, INPUT, TARGET) \
Test(ParserTests, NAME) \
{ \
	std::string Input = INPUT; \
	std::unique_ptr<DataSource> src = std::make_unique<StringSource>(Input); \
	cr_assert(src != NULL); \
	FunctionCaller caller; \
    caller.InstallOperators(); \
    caller.InstallFunctions(); \
\
	SyntaxTreeNode tree = Silikego::ParseInfix(std::move(src)); \
    Value target{TARGET}; \
	Value result{tree.Evaluate(caller)}; \
    cr_assert(target.Status() == result.Status()); \
    switch(result.Status()) \
    { \
    case ValueStatus::INTEGER: \
        cr_assert(target.Integer() == result.Integer()); \
        break; \
    case ValueStatus::FLOAT: \
        cr_assert(target.Float() == result.Float()); \
        break; \
    default: \
        break; \
    } \
}

ParserTest(TwoPlusThreeIsfive, "2 + 3", 5)
ParserTest(TwoPlusThreeIsfiveWithFloats, "2.0 + 3.0", 5.0)
ParserTest(FunctionAndParens, "sin(0.0) + 4", 4.0)
ParserTest(LostOfTimes, "multiply(2, 3, 5)", 30)
