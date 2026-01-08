#include <criterion/criterion.h>
#include <SilikegoCore/Node.h>
#include <SilikegoCore/Evaluate.h>

using Silikego::Node;
using Silikego::Value;
using Silikego::ValueStatus;

Test(NodeTests, NewIntZero) {
    Node node{0LL};

    Silikego::Engine caller;
    Value result = Silikego::Evaluate(caller, node);
    cr_assert(result.status() == ValueStatus::Integer);
    cr_assert(result.integer() == 0LL);
}

Test(NodeTests, NewFloatZero) {
    Node node{0.0};

    Silikego::Engine caller;
    Value result = Silikego::Evaluate(caller, node);
    cr_assert(result.status() == ValueStatus::Real);
    cr_assert(result.real() == 0.0);
}
