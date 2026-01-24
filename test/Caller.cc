#include <criterion/criterion.h>
#include <SilikegoCore/Engine.h>
#include <SilikegoCore/Math.h>
#include <SilikegoCore/Operation.h>

using std::vector;
using Silikego::Error;
using Silikego::Value;
using Silikego::ValueStatus;
using Silikego::Engine;
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

Test(EngineTests, NewCaller) {
    Engine caller;
}

Test(EngineTests, GetIntFunction) {
    Engine caller;
    Function *test = new PureFunction(GetFortyTwoInt);
    caller.installFunction("gfti", std::unique_ptr<Function>(test));
    Function* result = caller.fetchFunction("gfti");
    cr_assert(test == result);
}

Test(EngineTests, GetFloatFunction) {
    Engine caller;
    Function *test = new PureFunction(GetFortyTwoFloat);
    caller.installFunction("gftf", std::unique_ptr<Function>(test));
    Function* result = caller.fetchFunction("gftf");
    cr_assert(test == result);
}

Test(EngineTests, UseIntFunction) {
    Engine caller;
    caller.installFunction("gftf", std::make_unique<PureFunction>(GetFortyTwoInt));
    Value result = caller.callFunction("gftf", vector<Value>());
    cr_assert(result.status() == ValueStatus::Integer);
    cr_assert(result.integer() == 42LL);
}

Test(EngineTests, UseFloatFunction) {
    Engine caller;
    caller.installFunction("gftf", std::make_unique<PureFunction>(GetFortyTwoFloat));
    Value result = caller.callFunction("gftf", vector<Value>());
    cr_assert(result.status() == ValueStatus::Real);
    cr_assert(result.real() == 42.0);
}

Test(EngineTests, HandleBadFunction)
{
    Engine caller;
    Value result = caller.callFunction("bogus", vector<Value>());
    cr_assert(result.status() == ValueStatus::Error);
    cr_assert(result.error() == Error::FunctionName);
}

Test(EngineTests, FunctionsInstall)
{
    Engine caller{};
    InstallMathFunctions(caller);
}

Test(EngineTests, OperationsInstall)
{
    Engine caller{};
    InstallOperations(caller);
}
