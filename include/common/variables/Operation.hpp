#pragma once
#include <sstream>

template <class T>
class Value;

template <class T>
class Operation {
public:
    virtual T evaluate() = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string stringify() const = 0;
};

template <class T>
class Evaluation: public Operation<T> {
private:
    Value<T> inputVar;
public:
    Evaluation(Value<T> inputVar)
    : inputVar(inputVar) {};
    virtual T evaluate() {
        return inputVar.getValue();
    };
    virtual std::string getDescription() const {
        return "Returns the latest value of the inputted variable";
    };
    virtual std::string stringify() const {
        std::ostringstream oss;
        oss << inputVar.getValue();
        return oss.str();
    };
};

