#include <criterion/criterion.h>
#include <SilikegoCore/Value.h>

using std::unique_ptr;
using std::make_unique;
using std::vector;
using Silikego::Error;
using Silikego::Value;
using Silikego::ValueStatus;


#define VALUE_TEST(TEST_NAME, VALUE) \
Test(ValueTests, TEST_NAME) \
{ \
    Value test_value{VALUE}; \
    cr_assert(test_value.status() == _Generic((VALUE), \
        Silikego::Error:  ValueStatus::Error, \
        int:              ValueStatus::Integer, \
        long long int:    ValueStatus::Integer, \
        double:           ValueStatus::Real \
    )); \
    cr_assert(_Generic((VALUE), \
        Silikego::Error: test_value.toError(),   \
        int:             test_value.toInteger(), \
        long long int:   test_value.toInteger(), \
        double:          test_value.toReal()     \
    ) == VALUE); \
}

VALUE_TEST(NewIntZero, 0LL)
VALUE_TEST(NewIntFortyTwo, 42LL)
VALUE_TEST(NewIntNegativeOne, -1LL)
VALUE_TEST(NewRealZero, 0.0)
VALUE_TEST(NewRealOneeighth, 0.125)
VALUE_TEST(NewRealNegativeZero, -0.0)
VALUE_TEST(NewRealFortyTwo, 42.0)
VALUE_TEST(NewRealNegativeOne, -1.0)
VALUE_TEST(NewErrorSyntax, Error::Syntax)
VALUE_TEST(NewErrorFunctionLookup, Error::FunctionName)
VALUE_TEST(NewErrorFunctionArguments, Error::FunctionArguments)
VALUE_TEST(NewErrorDomain, Error::Domain)
VALUE_TEST(NewErrorRange, Error::Range)
