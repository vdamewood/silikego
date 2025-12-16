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
        result.toError() == ERROR), \
        __VA_ARGS__ \
)

#define TestFunctionForEpsilon(NAME, FUNCTION, RESULT, EPSILON, ...) \
    TestFunction(NAME, FUNCTION, \
        ValueStatus::Real, \
        (std::fabs(result.toReal() - RESULT) <= EPSILON), \
        __VA_ARGS__ \
)

#define TestFunctionForEquality(NAME, FUNCTION, RESULT, ...) \
    TestFunction(NAME, FUNCTION, \
        _Generic((RESULT), \
            int:                    ValueStatus::Integer, \
            long long int:          ValueStatus::Integer, \
            float:                  ValueStatus::Real,    \
            double:                 ValueStatus::Real    \
        ), \
        (_Generic((RESULT), \
            int:           result.toInteger(), \
            long long int: result.toInteger(), \
            float:         result.toReal(),    \
            double:        result.toReal()     \
        ) == RESULT), \
        __VA_ARGS__ \
)
