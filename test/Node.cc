#include <criterion/criterion.h>
#include <SilikegoCore/SyntaxTree.h>

using Silikego::SyntaxTreeNode;
using Silikego::Value;
using Silikego::ValueStatus;

Test(SyntaxTreeTests, NewIntZero) {
    SyntaxTreeNode node{0LL};

    Silikego::FunctionCaller caller;
    Value result = node.Evaluate(caller);
    cr_assert(result.status() == ValueStatus::Integer);
    cr_assert(result.asInteger() == 0LL);
}

Test(SyntaxTreeTests, NewFloatZero) {
    SyntaxTreeNode node{0.0};

    Silikego::FunctionCaller caller;
    Value result = node.Evaluate(caller);
    cr_assert(result.status() == ValueStatus::Float);
    cr_assert(result.asFloat() == 0.0);
}
