#if !defined SUITE
#error Must define SUITE to use FunctionMacros.h
#endif

#if !defined INSTALL_FUNCTION
#error Must define INSTALL_FUNCTION to use FunctionMacros.h
#endif

using Silikego::ValueStatus;

#define TestFunction(NAME, FUNCTION, STATUS, RESULT_EXPRESSION, ...) \
Test(SUITE, NAME) \
{ \
    Silikego::FunctionCaller caller; \
    INSTALL_FUNCTION(caller); \
    std::vector<Silikego::Value> input \
    { \
        __VA_ARGS__ \
    }; \
    Silikego::Value result = caller.call( \
        FUNCTION, \
        input); \
    cr_assert(result.status() == STATUS, \
        "Status: Expected: %d; Got %d;", STATUS, result.status()); \
    cr_assert(RESULT_EXPRESSION); \
}

#define TestFunctionForError(NAME, FUNCTION, ERROR, ...) \
    TestFunction(NAME, FUNCTION, \
        ValueStatus::Error, \
        (result.asError() == ERROR), \
        __VA_ARGS__ \
)

#define TestFunctionForEpsilon(NAME, FUNCTION, RESULT, EPSILON, ...) \
    TestFunction(NAME, FUNCTION, \
        ValueStatus::Float, \
        (std::fabs(result.asFloat() - RESULT) <= EPSILON), \
        __VA_ARGS__ \
)

#define TestFunctionForEquality(NAME, FUNCTION, RESULT, ...) \
    TestFunction(NAME, FUNCTION, \
        _Generic((RESULT), \
            signed char:            ValueStatus::Integer, \
            short int:              ValueStatus::Integer, \
            int:                    ValueStatus::Integer, \
            long int:               ValueStatus::Integer, \
            long long int:          ValueStatus::Integer, \
            float:                  ValueStatus::Float,   \
            double:                 ValueStatus::Float,   \
            default:                ValueStatus::Integer  \
        ), \
        (_Generic((RESULT), \
            signed char:            result.asInteger(), \
            short int:              result.asInteger(), \
            int:                    result.asInteger(), \
            long int:               result.asInteger(), \
            long long int:          result.asInteger(), \
            float:                  result.asFloat(),   \
            double:                 result.asFloat(),   \
            default:                result.asInteger()  \
        ) == RESULT), \
        __VA_ARGS__ \
)
