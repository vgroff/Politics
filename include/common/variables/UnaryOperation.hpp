#pragma once
#include "Operation.hpp"

template <class T, class U>
class UnaryOperation: public Operation<T> {
private:
    std::shared_ptr<Operation<U>> inputOp = nullptr;
public:
    UnaryOperation(std::shared_ptr<Operation<U>> inputOp)
    : inputOp(inputOp) {};
    virtual T evaluate() {
        return evaluate(inputOp->evaluate());
    };
    virtual T evaluate(U input) = 0;
};