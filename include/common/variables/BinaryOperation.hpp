#pragma once
#include "Operation.hpp"


template <class T, class U, class V>
class BinaryOperation: public Operation<T>  {
private:
    std::shared_ptr<Operation<U>> inputOp1 = nullptr;
    std::shared_ptr<Operation<V>> inputOp2 = nullptr;
public:
    BinaryOperation(std::shared_ptr<Operation<U>> inputOp1, std::shared_ptr<Operation<V>> inputOp2)
    : inputOp1(inputOp1), inputOp2(inputOp2) {};
    BinaryOperation(Value<T> inputVar1, std::shared_ptr<Operation<T>> inputOp2)
    : inputOp2(inputOp2) {
        inputOp1 = std::make_shared<Evaluation<U>>(inputVar1);
    };
    BinaryOperation(std::shared_ptr<Operation<T>> inputVar1, Value<T> inputVar2)
    : inputOp1(inputOp1) {
        inputOp2 = std::make_shared<Evaluation<V>>(inputVar2);
    };
    BinaryOperation(Value<T> inputVar1, Value<T> inputVar2)
    {
        inputOp1 = std::make_shared<Evaluation<U>>(inputVar1);
        inputOp2 = std::make_shared<Evaluation<V>>(inputVar2);
    };

    virtual std::string stringify() const {
        std::ostringstream oss;
        oss << inputOp1->evaluate() << stringifyOperator() << inputOp2->evaluate();
        return oss.str();
    };
    virtual std::string stringifyOperator() const = 0;

    T evaluate() {
        return evaluate(inputOp1->evaluate(), inputOp2->evaluate());
    };
    virtual T evaluate(U x, V y) = 0;
};

template <class T>
class Addition: public BinaryOperation<T, T, T> {
public:
    using BinaryOperation<T,T,T>::BinaryOperation;
    virtual T evaluate(T x, T y) {
        return x + y;
    };
    virtual std::string getDescription() const {
        return "Returns the sum of two inputted variables";
    };
    virtual std::string stringifyOperator() const {
        return "+";
    };
};

template <class T, class U>
class Multiplication: public BinaryOperation<T, T, U> {
public:
    using BinaryOperation<T,T,U>::BinaryOperation;
    virtual T evaluate(T x, U y) {
        return x*y;
    };
    virtual std::string getDescription() const {
        return "Returns the multiplication of two inputted variables";
    };
    virtual std::string stringifyOperator() const {
        return "*";
    };
};
