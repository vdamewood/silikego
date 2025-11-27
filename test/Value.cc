#include <criterion/criterion.h>
#include <SilikegoCore/Value.h>

using std::unique_ptr;
using std::make_unique;
using std::vector;
using Silikego::Value;
using Silikego::ValueStatus;

Test(ValueTests, NewIntZero) {
    Value value(0LL);
    cr_assert(value.Status() == ValueStatus::INTEGER);
    cr_assert(value.Integer() == 0LL);
}

Test(ValueTests, NewIntFortyTwo) {
    Value value(42LL);
    cr_assert(value.Status() == ValueStatus::INTEGER);
    cr_assert(value.Integer() == 42LL);
}

Test(ValueTests, NewIntNegativeOne) {
    Value value(-1LL);
    cr_assert(value.Status() == ValueStatus::INTEGER);
    cr_assert(value.Integer() == -1LL);
}

Test(ValueTests, NewFloatZero) {
    Value value(0.0);
    cr_assert(value.Status() == ValueStatus::FLOAT);
    cr_assert(value.Integer() == 0.0);
}

Test(ValueTests, NewFloatNegativeZero) {
    Value value(-0.0);
    cr_assert(value.Status() == ValueStatus::FLOAT);
    cr_assert(value.Integer() == -0.0);
}

Test(ValueTests, NewFloatEighth) {
    Value value(0.125);
    cr_assert(value.Status() == ValueStatus::FLOAT);
    cr_assert(value.Float() == 0.125);
}

Test(ValueTests, NewFloatFortyTwo) {
    Value value(42.0);
    cr_assert(value.Status() == ValueStatus::FLOAT);
    cr_assert(value.Integer() == 42.0);
}

Test(ValueTests, NewFloatNegativeOne) {
    Value value(-1.0);
    cr_assert(value.Status() == ValueStatus::FLOAT);
    cr_assert(value.Integer() == -1.0);
}

Test(ValueTests, NewErrorMemory)
{
    Value value(ValueStatus::MEMORY_ERR);
	cr_assert(value.Status() == ValueStatus::MEMORY_ERR);
}

Test(ValueTests, NewErrorSyntax)
{
    Value value(ValueStatus::SYNTAX_ERR);
	cr_assert(value.Status() == ValueStatus::SYNTAX_ERR);
}

Test(ValueTests, NewErrorFunctionLookup)
{
    Value value(ValueStatus::BAD_FUNCTION);
	cr_assert(value.Status() == ValueStatus::BAD_FUNCTION);
}

Test(ValueTests, NewErrorFunctionArguments)
{
    Value value(ValueStatus::BAD_ARGUMENTS);
	cr_assert(value.Status() == ValueStatus::BAD_ARGUMENTS);
}

Test(ValueTests, NewErrorFunctionDomain)
{
    Value value(ValueStatus::DOMAIN_ERR);
	cr_assert(value.Status() == ValueStatus::DOMAIN_ERR);
}

Test(ValueTests, NewErrorFunctionRange)
{
    Value value(ValueStatus::RANGE_ERR);
	cr_assert(value.Status() == ValueStatus::RANGE_ERR);
}
