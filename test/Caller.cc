#include <criterion/criterion.h>
#include <SilikegoCore/FunctionCaller.h>

using std::vector;
using Silikego::Error;
using Silikego::Value;
using Silikego::ValueStatus;
using Silikego::FunctionCaller;
using Silikego::Function;
using Silikego::PureFunction;

Value GetFortyTwoInt(const vector<Value>& ArgV)
{
    (void)ArgV;
    return Value{42LL};
}

Value GetFortyTwoFloat(const vector<Value>& ArgV)
{
    (void)ArgV;
    return Value{42.0};
}

Test(FunctionCallerTests, NewCaller) {
    FunctionCaller caller;
}

Test(FunctionCallerTests, GetIntFunction) {
    FunctionCaller caller;
    Function *test = new PureFunction(GetFortyTwoInt);
    caller.install("gfti", std::unique_ptr<Function>(test));
    Function* result = caller.fetch("gfti");
    cr_assert(test == result);
}

Test(FunctionCallerTests, GetFloatFunction) {
    FunctionCaller caller;
    Function *test = new PureFunction(GetFortyTwoFloat);
    caller.install("gftf", std::unique_ptr<Function>(test));
    Function* result = caller.fetch("gftf");
    cr_assert(test == result);
}

Test(FunctionCallerTests, UseIntFunction) {
    FunctionCaller caller;
    caller.install("gftf", std::make_unique<PureFunction>(GetFortyTwoInt));
    Value result = caller.call("gftf", vector<Value>());
    cr_assert(result.status() == ValueStatus::Integer);
    cr_assert(result.integer() == 42LL);
}

Test(FunctionCallerTests, UseFloatFunction) {
    FunctionCaller caller;
    caller.install("gftf", std::make_unique<PureFunction>(GetFortyTwoFloat));
    Value result = caller.call("gftf", vector<Value>());
    cr_assert(result.status() == ValueStatus::Real);
    cr_assert(result.real() == 42.0);
}

Test(FunctionCallerTests, HandleBadFunction)
{
    FunctionCaller caller;
    Value result = caller.call("bogus", vector<Value>());
    cr_assert(result.status() == ValueStatus::Error);
    cr_assert(result.error() == Error::FunctionName);
}

Test(FunctionCallerTests, FunctionsInstall)
{
    FunctionCaller caller{};
    InstallFunctions(caller);
}

Test(FunctionCallerTests, OperatorsInstall)
{
    FunctionCaller caller{};
    InstallOperators(caller);
}
