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
    Value<T> inputVal;
public:
    Evaluation(Value<T> inputVal)
    : inputVal(inputVal) {};
    virtual T evaluate() {
        return inputVal.getValue();
    };
    virtual std::string getDescription() const {
        return "Returns the latest value of the inputted variable";
    };
    virtual std::string stringify() const {
        std::ostringstream oss;
        oss << inputVal.getValue();
        return oss.str();
    };
};

