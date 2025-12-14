#include <criterion/criterion.h>
#include <SilikegoCore/SyntaxTree.h>

using Silikego::SyntaxTreeNode;
using Silikego::Value;
using Silikego::ValueStatus;

Test(SyntaxTreeTests, NewIntZero) {
    SyntaxTreeNode node{0LL};

    Silikego::FunctionCaller caller;
    Value result = node.evaluate(caller);
    cr_assert(result.status() == ValueStatus::Integer);
    cr_assert(static_cast<long long int>(result) == 0LL);
}

Test(SyntaxTreeTests, NewFloatZero) {
    SyntaxTreeNode node{0.0};

    Silikego::FunctionCaller caller;
    Value result = node.evaluate(caller);
    cr_assert(result.status() == ValueStatus::Real);
    cr_assert(static_cast<double>(result) == 0.0);
}
