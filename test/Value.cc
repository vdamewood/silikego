#include <criterion/criterion.h>
#include <SilikegoCore/Value.h>

using std::unique_ptr;
using std::make_unique;
using std::vector;
using Silikego::Error;
using Silikego::Value;
using Silikego::ValueStatus;

Test(ValueTests, NewIntZero) {
    Value value(0LL);
    cr_assert(value.status() == ValueStatus::Integer);
    cr_assert(value.asInteger() == 0LL);
}

Test(ValueTests, NewIntFortyTwo) {
    Value value(42LL);
    cr_assert(value.status() == ValueStatus::Integer);
    cr_assert(value.asInteger() == 42LL);
}

Test(ValueTests, NewIntNegativeOne) {
    Value value(-1LL);
    cr_assert(value.status() == ValueStatus::Integer);
    cr_assert(value.asInteger() == -1LL);
}

Test(ValueTests, NewFloatZero) {
    Value value(0.0);
    cr_assert(value.status() == ValueStatus::Float);
    cr_assert(value.asInteger() == 0.0);
}

Test(ValueTests, NewFloatNegativeZero) {
    Value value(-0.0);
    cr_assert(value.status() == ValueStatus::Float);
    cr_assert(value.asInteger() == -0.0);
}

Test(ValueTests, NewFloatEighth) {
    Value value(0.125);
    cr_assert(value.status() == ValueStatus::Float);
    cr_assert(value.asFloat() == 0.125);
}

Test(ValueTests, NewFloatFortyTwo) {
    Value value(42.0);
    cr_assert(value.status() == ValueStatus::Float);
    cr_assert(value.asInteger() == 42.0);
}

Test(ValueTests, NewFloatNegativeOne) {
    Value value(-1.0);
    cr_assert(value.status() == ValueStatus::Float);
    cr_assert(value.asInteger() == -1.0);
}

Test(ValueTests, NewErrorMemory)
{
    Value value(Error::Memory);
	cr_assert(value.status() == ValueStatus::Error);
	cr_assert(value.asError() == Error::Memory);
}

Test(ValueTests, NewErrorSyntax)
{
    Value value(Error::Syntax);
	cr_assert(value.status() == ValueStatus::Error);
	cr_assert(value.asError() == Error::Syntax);
}

Test(ValueTests, NewErrorFunctionLookup)
{
    Value value(Error::FunctionName);
	cr_assert(value.status() == ValueStatus::Error);
	cr_assert(value.asError() == Error::FunctionName);
}

Test(ValueTests, NewErrorFunctionArguments)
{
    Value value(Error::FunctionArguments);
	cr_assert(value.status() == ValueStatus::Error);
	cr_assert(value.asError() == Error::FunctionArguments);
}

Test(ValueTests, NewErrorFunctionDomain)
{
    Value value(Error::Domain);
	cr_assert(value.status() == ValueStatus::Error);
	cr_assert(value.asError() == Error::Domain);
}

Test(ValueTests, NewErrorFunctionRange)
{
    Value value(Error::Range);
	cr_assert(value.status() == ValueStatus::Error);
	cr_assert(value.asError() == Error::Range);
}
