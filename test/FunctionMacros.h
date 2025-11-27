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
    caller.INSTALL_FUNCTION(); \
    std::vector<Silikego::Value> input \
    { \
        __VA_ARGS__ \
    }; \
    Silikego::Value result = caller.Call( \
        FUNCTION, \
        input); \
    cr_assert(result.Status() == STATUS, \
        "Status: Expected: %d; Got %d;", STATUS, result.Status()); \
    cr_assert(RESULT_EXPRESSION); \
}

#define TestFunctionForStatus(NAME, FUNCTION, STATUS, ...) \
    TestFunction(NAME, FUNCTION, \
        STATUS, \
        (result.Integer() == 0), \
        __VA_ARGS__ \
)

#define TestFunctionForEpsilon(NAME, FUNCTION, RESULT, EPSILON, ...) \
    TestFunction(NAME, FUNCTION, \
        ValueStatus::FLOAT, \
        (std::fabs(result.Float() - RESULT) <= EPSILON), \
        __VA_ARGS__ \
)

#define TestFunctionForEquality(NAME, FUNCTION, RESULT, ...) \
    TestFunction(NAME, FUNCTION, \
        _Generic((RESULT), \
            signed char:            ValueStatus::INTEGER, \
            short int:              ValueStatus::INTEGER, \
            int:                    ValueStatus::INTEGER, \
            long int:               ValueStatus::INTEGER, \
            long long int:          ValueStatus::INTEGER, \
            float:                  ValueStatus::FLOAT,   \
            double:                 ValueStatus::FLOAT,   \
            default:                ValueStatus::INTEGER  \
        ), \
        (_Generic((RESULT), \
            signed char:            result.Integer(), \
            short int:              result.Integer(), \
            int:                    result.Integer(), \
            long int:               result.Integer(), \
            long long int:          result.Integer(), \
            float:                  result.Float(),   \
            double:                 result.Float(),   \
            default:                result.Integer()  \
        ) == RESULT), \
        __VA_ARGS__ \
)
