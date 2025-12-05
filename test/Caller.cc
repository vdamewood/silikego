#include <criterion/criterion.h>
#include <SilikegoCore/FunctionCaller.h>

using std::unique_ptr;
using std::make_unique;
using std::vector;
using Silikego::Error;
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
    unique_ptr<FunctionCaller> caller = make_unique<FunctionCaller>();
    cr_assert(caller != nullptr);
}

Test(FunctionCallerTests, GetIntFunction) {
    unique_ptr<FunctionCaller> caller = make_unique<FunctionCaller>();
    cr_assert(caller != nullptr);
    caller->Install("gfti", GetFortyTwoInt);
    Silikego::FunctionCaller::FunctionPointer result = caller->Get("gfti");
    cr_assert(result == GetFortyTwoInt);
}

Test(FunctionCallerTests, GetFloatFunction) {
    unique_ptr<FunctionCaller> caller = make_unique<FunctionCaller>();
    cr_assert(caller != nullptr);
    caller->Install("gftf", GetFortyTwoFloat);
    Silikego::FunctionCaller::FunctionPointer result = caller->Get("gftf");
    cr_assert(result == GetFortyTwoFloat);
}

Test(FunctionCallerTests, UseFloatFunction) {
    unique_ptr<FunctionCaller> caller = make_unique<FunctionCaller>();
    cr_assert(caller != nullptr);
    caller->Install("gftf", GetFortyTwoFloat);
    Value result = caller->Call("gftf", vector<Value>());
    cr_assert(result.status() == ValueStatus::Float);
    cr_assert(result.asFloat() == 42.0);
}

Test(FunctionCallerTests, HandleBadFunction)
{
    unique_ptr<FunctionCaller> caller = make_unique<FunctionCaller>();
    cr_assert(caller != nullptr);
    Value result = caller->Call("bogus", vector<Value>());
    cr_assert(result.status() == ValueStatus::Error);
    cr_assert(result.asError() == Error::FunctionName);
}

Test(FunctionCallerTests, FunctionsInstall)
{
    unique_ptr<FunctionCaller> caller = make_unique<FunctionCaller>();
    cr_assert(caller != nullptr);
    caller->InstallFunctions();
    //cr_assert(Result);
}

Test(FunctionCallerTests, OperatorsInstall)
{
    unique_ptr<FunctionCaller> caller = make_unique<FunctionCaller>();
    cr_assert(caller != nullptr);
    caller->InstallOperators();
    //cr_assert(Result);
}
