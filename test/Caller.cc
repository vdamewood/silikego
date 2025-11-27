#include <criterion/criterion.h>
#include <SilikegoCore/FunctionCaller.h>

using std::unique_ptr;
using std::make_unique;
using std::vector;
using Silikego::Value;
using Silikego::ValueStatus;
using Silikego::FunctionCaller;

Value GetFortyTwoInt(vector<Value> ArgV)
{
    (void)ArgV;
    return Value(42LL);
}

Value GetFortyTwoFloat(vector<Value> ArgV)
{
    (void)ArgV;
    return Value(42.0);
}

Test(FunctionCallerTests, NewCaller) {
    unique_ptr<FunctionCaller> Caller = make_unique<FunctionCaller>();
    cr_assert(Caller != nullptr);
}

Test(FunctionCallerTests, UseIntFunction) {
    unique_ptr<FunctionCaller> Caller = make_unique<FunctionCaller>();
    cr_assert(Caller != nullptr);
    Caller->Install("gfti", GetFortyTwoInt);
    Value TestValue = Caller->Call("gfti", vector<Value>());
    cr_assert(TestValue.Status() == ValueStatus::INTEGER);
    cr_assert(TestValue.Integer() == 42);
}

Test(FunctionCallerTests, UseFloatFunction) {
    unique_ptr<FunctionCaller> Caller = make_unique<FunctionCaller>();
    cr_assert(Caller != nullptr);
    Caller->Install("gftf", GetFortyTwoFloat);
    Value TestValue = Caller->Call("gftf", vector<Value>());
    cr_assert(TestValue.Status() == ValueStatus::FLOAT);
    cr_assert(TestValue.Float() == 42.0);
}

Test(FunctionCallerTests, HandleBadFunction)
{
    unique_ptr<FunctionCaller> Caller = make_unique<FunctionCaller>();
    cr_assert(Caller != nullptr);
    Value TestValue = Caller->Call("bogus", vector<Value>());
    cr_assert(TestValue.Status() == ValueStatus::BAD_FUNCTION);
}

Test(FunctionCallerTests, FunctionsInstall)
{
    unique_ptr<FunctionCaller> Caller = make_unique<FunctionCaller>();
    cr_assert(Caller != nullptr);
    Caller->InstallFunctions();
    //cr_assert(Result);
}

Test(FunctionCallerTests, OperatorsInstall)
{
    unique_ptr<FunctionCaller> Caller = make_unique<FunctionCaller>();
    cr_assert(Caller != nullptr);
    Caller->InstallOperators();
    //cr_assert(Result);
}
