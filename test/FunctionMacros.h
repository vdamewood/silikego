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
    Silikego::Engine caller; \
    INSTALL_FUNCTION(caller); \
    std::vector<Silikego::Value> input \
    { \
        __VA_ARGS__ \
    }; \
    Silikego::Value result = caller.callFunction( \
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
        (std::fabs(result.real() - RESULT) <= EPSILON), \
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
            int:           result.integer(), \
            long long int: result.integer(), \
            float:         result.real(),    \
            double:        result.real()     \
        ) == RESULT), \
        __VA_ARGS__ \
)
