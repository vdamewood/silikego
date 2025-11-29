#include <criterion/criterion.h>
#include <SilikegoCore/SyntaxTree.h>

using Silikego::SyntaxTreeNode;
using Silikego::Value;
using Silikego::ValueStatus;

Test(SyntaxTreeTests, NewIntZero) {
    SyntaxTreeNode Node{0LL};

    Silikego::FunctionCaller caller;
    Value result = Node.Evaluate(caller);
    cr_assert(result.Status() == ValueStatus::INTEGER);
    cr_assert(result.Integer() == 0LL);
}

Test(SyntaxTreeTests, NewFloatZero) {
    SyntaxTreeNode Node{0.0};

    Silikego::FunctionCaller caller;
    Value result = Node.Evaluate(caller);
    cr_assert(result.Status() == ValueStatus::FLOAT);
    cr_assert(result.Integer() == 0.0);
}
