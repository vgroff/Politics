#pragma once
#include "Modifier.hpp"
#include <sstream>

template <class T>
class Operation {
public:
    virtual T evaluate() = 0;
    virtual std::string getDescription() = 0;
    virtual std::string stringify() = 0;
};

template <class T>
class EvaluateOperation: Operation<T> {
private:
    std::shared_ptr<Variable<T>> inputVar = nullptr;
public:
    EvaluateOperation(std::shared_ptr<Variable<T>> inputVar)
    : inputVar(inputVar) {};
    virtual T evaluate() {
        return inputVar->getLatestValue();
    };
    virtual std::string getDescription() {
        return "Returns the latest value of the inputted variable";
    };
    virtual std::string stringify() {
        std::ostringstream oss;
        oss << inputVar->getLatestValue();
        return oss.str();
    };
};

template <class T, class U>
class UnaryOperation: Operation<T> {
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

template <class T, class U, class V>
class BinaryOperation: Operation<T>  {
private:
    std::shared_ptr<Operation<U>> inputOp1 = nullptr;
    std::shared_ptr<Operation<V>> inputOp2 = nullptr;
public:
    BinaryOperation(std::shared_ptr<Operation<U>> inputOp1, std::shared_ptr<Operation<V>> inputOp2)
    : inputOp1(inputOp1), inputOp2(inputOp2) {};
    BinaryOperation(std::shared_ptr<Variable<T>> inputVar1, std::shared_ptr<Operation<T>> inputOp2)
    : inputOp2(inputOp2) {
        inputOp1 = std::make_shared<EvaluateOperation<U>>(inputVar1);
    };
    BinaryOperation(std::shared_ptr<Operation<T>> inputVar1, std::shared_ptr<Variable<T>> inputVar2)
    : inputOp1(inputOp1) {
        inputOp2 = std::make_shared<EvaluateOperation<V>>(inputVar2);
    };
    BinaryOperation(std::shared_ptr<Variable<T>> inputVar1, std::shared_ptr<Variable<T>> inputVar2)
    {
        inputOp1 = std::make_shared<EvaluateOperation<U>>(inputVar1);
        inputOp2 = std::make_shared<EvaluateOperation<V>>(inputVar2);
    };

    virtual std::string stringify() {
        std::ostringstream oss;
        oss << inputOp1->evaluate() << stringifyOperator() << inputOp2-evaluate();
        return oss.str();
    };
    virtual std::string stringifyOperator() = 0;

    T evaluate() {
        evaluate(inputOp1->evaluate(), inputOp2->evaluate());
    };
    virtual T evaluate(U x, V y) = 0;
};

template <class T>
class AdditiveOperation: BinaryOperation<T, T, T> {
public:
    using BinaryOperation<T,T,T>::BinaryOperation;
    virtual T evaluate(T x, T y) {
        return x + y;
    };
    virtual std::string getDescription() {
        return "Returns the sum of two inputted variables";
    };
    virtual std::string stringifyOperator() {
        return "+";
    };
};

template <class T>
class MultiplicativeOperation: BinaryOperation<T, T, T> {
public:
    using BinaryOperation<T,T,T>::BinaryOperation;
    virtual T evaluate(T x, T y) {
        return x*y;
    };
    virtual std::string getDescription() {
        return "Returns the multiplication of two inputted variables";
    };
    virtual std::string stringifyOperator() {
        return "*";
    };
};