#include <criterion/criterion.h>
#include <SilikegoCore/SyntaxTree.h>

using Silikego::SyntaxTreeNode;
using Silikego::LeafNode;
using Silikego::Value;
using Silikego::ValueStatus;

using std::unique_ptr;
using std::make_unique;

Test(SyntaxTreeTests, NewIntZero) {
    std::unique_ptr<Silikego::SyntaxTreeNode> Node = make_unique<LeafNode>(Value(0));
    //cr_assert(SilikoSyntaxTreeGetType(Node) == SILIKO_AST_LEAF);

    Silikego::FunctionCaller caller;
    Value result = Node->Evaluate(caller);
    cr_assert(result.Status() == ValueStatus::INTEGER);
    cr_assert(result.Integer() == 0L);
}

Test(SyntaxTreeTests, NewFloatZero) {
    std::unique_ptr<Silikego::SyntaxTreeNode> Node = make_unique<LeafNode>(Value(0.0));
    //cr_assert(SilikoSyntaxTreeGetType(Node) == SILIKO_AST_LEAF);

    Silikego::FunctionCaller caller;
    Value result = Node->Evaluate(caller);
    cr_assert(result.Status() == ValueStatus::FLOAT);
    cr_assert(result.Integer() == 0.0);
}
