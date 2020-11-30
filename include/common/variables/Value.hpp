#pragma once
#include "./Bounding.hpp"
#include "./Value.hpp"
#include <exception>


class MalformedBoundsException : public std::exception
{
	const char * what () const throw ()
    {
    	return "MalformedBoundsException";
    }
};

class SettingOutOfBounds : public std::exception
{
	const char * what () const throw ()
    {
    	return "SettingOutOfBounds";
    }
};


template <class T>
class Variable;

template <class T>
class Value {
private:
    std::string name;
    Variable<T>* parentVariable = nullptr;
    T value;
    std::shared_ptr<T> lowerBound = nullptr;
    std::shared_ptr<T> upperBound = nullptr;
public:
    Value(std::string name, T value)
    : name(name), value(value) {};
    Value(std::string name, T value, Variable<T>* parentVariable)
    : name(name), value(value), parentVariable(parentVariable) {};
    Value(std::string name, T value, std::pair<T,T> bounds_, bool softSetOnInit = false, Variable<T>* parentVariable = nullptr)
    : name(name), parentVariable(parentVariable) {
        if (!exceedsLowerBound(bounds_.second, bounds_.first) || !exceedsUpperBound(bounds_.first, bounds_.second)) {
            throw MalformedBoundsException();
        }
        upperBound = std::make_shared<T>(bounds_.second);
        lowerBound = std::make_shared<T>(bounds_.first);
        if (softSetOnInit) {
            softSetValue(value);
        } else {
            setValue(value);
        }
    };
    Value(std::string name, T defaultValue, T lowerBound, Variable<T>* parentVariable, bool softSetOnInit = false)
    : name(name), value(defaultValue), parentVariable(parentVariable) {
        lowerBound = std::make_shared<T>(lowerBound);
        if (softSetOnInit) {
            softSetValue(value);
        } else {
            setValue(value);
        }
    };
    // Value(std::string name, T defaultValue, std::shared_ptr<std::pair<T,T>> bounds_, Variable<T>* parentVariable)
    // : name(name), value(defaultValue), parentVariable(parentVariable) {
    //     if (bounds_ != nullptr) {
    //         if (bounds_->second <= bounds_->first) {
    //             throw MalformedBoundsException();
    //         }
    //         bounds = bounds_;
    //     }
    // };
    std::string getName() const {
        return name;
    };
    std::string setName(std::string newName) {
        name = newName;
    };
    T getValue() const {
        return value;
    };
    void setValue(T newValue) {
        if (upperBound && exceedsHigherBound<T>(newValue, *upperBound)) {
            throw SettingOutOfBounds();
        } else if (lowerBound && exceedsLowerBound<T>(newValue, *lowerBound)) {
            throw SettingOutOfBounds();
        } else {
            value = newValue;
        }
        value = newValue;
    };
    void softSetValue(T newValue) {
        if (upperBound && exceedsHigherBound<T>(newValue, *upperBound)) {
            newValue = *upperBound;
        } else if (lowerBound && exceedsLowerBound<T>(newValue, *lowerBound)) {
            newValue = *lowerBound;
        } else {
            value = newValue;
        }
    };
    Variable<T>* getParent() const {
        return parentVariable;
    };
    void setParent(Variable<T>* parentVariable) const {
        parentVariable = parentVariable;
    };
};


template <class T>
class OperationList {
    std::vector<std::shared_ptr<Operation<T>>> operations = {};
public:
    OperationList() {};
    OperationList(std::vector<std::shared_ptr<Operation<T>>> operations)
    : operations(operations) {};
    std::vector<std::shared_ptr<Operation<T>>> getOperations() {
        return operations;
    };
    T getResult() {
        // The last operation in the list gives the final output
        return operations[operations.size() - 1]->evaluate();
    }
    std::string stringify() {
        std::ostringstream oss;
        for (const auto& operation : operations) {
            oss << operation->stringify() << ", ";
        } 
        return oss.str();
    }
};

class TooManySettersException : public std::exception
{
	const char * what () const throw ()
    {
    	return "TooManySettersException";
    }
};

template <class T>
OperationList<T> evaluateModifiers(Value<T>& value, std::vector<std::shared_ptr<Modifier<T>>>& modifiers) {
    std::sort(modifiers.begin(), modifiers.end(), [](std::shared_ptr<Modifier<T>> modifier1, std::shared_ptr<Modifier<T>> modifier2) {
        return modifier1->getPriority() < modifier2->getPriority();
    });
    std::vector<std::shared_ptr<Operation<T>>> modifications = {};
    Value<T> currentValue = value;
    size_t numSetters = 0;
    for (auto& modifier : modifiers) {
        if (modifier->getPriority() == Setter) {
            numSetters++;
        }
        if (numSetters != 1) {
            throw TooManySettersException();
        }
        auto modification = modifier->getModification(currentValue);
        modifications.push_back(modification);
        currentValue.softSetValue(modification->evaluate());
    }
    OperationList derivedValue(modifications);
    return derivedValue;
}

template <class T>
class Calculation {
    OperationList<T> calculation;
    std::string description;
public:
    Calculation(std::string description): description(description) {};
    Calculation(std::string description, Value<T> value, std::vector<std::shared_ptr<Modifier<T>>> modifiers)
    : description(description) {
       calculation = evaluateModifiers(value, modifiers); 
    };
    Calculation(std::string description, OperationList<T> calculation)
    : description(description), calculation(calculation) {};
    std::vector<std::shared_ptr<Operation<T>>> getOperations() {
        return calculation;
    };
    T getResult() {
        // The last operation in the list gives the final output
        return calculation.getResult();
    }
    std::string stringify() {
        return calculation.stringify();
    }
};


template <class T>
class TrackedValue: public Value<T>, private ClockSubscriber {
private:
    std::vector<std::pair<time_point, T>> pastValues;
    size_t daysPast = 0;
public:
    TrackedValue(std::string variableName, T defaultValue, Variable<T>* parentVariable, std::weak_ptr<Clock> clock, size_t trackingRateDays = 7)
    : Value<T>(variableName, defaultValue, parentVariable), ClockSubscriber(clock) {
        subToClock(trackingRateDays);
    }
    TrackedValue(std::string variableName, T defaultValue, std::pair<T, T> bounds, Variable<T>* parentVariable, std::weak_ptr<Clock> clock, size_t trackingRateDays = 7) 
    : Value<T>(variableName, defaultValue, bounds, parentVariable), ClockSubscriber(clock) {
        subToClock(trackingRateDays);
    }

    std::vector<std::pair<time_point, Calculation<T>>> getHistory() {
        return pastValues;
    }

    void subToClock(size_t trackingRateDays) {
        if (trackingRateDays > 0) {
            subscribeToClock([trackingRateDays, this](time_point currentDate, size_t numDays) {
                daysPast += numDays;
                if (trackingRateDays >= this->daysPast) {
                    pastValues.push_back({currentDate, this->getValue()});
                    daysPast = 0;
                }
            });
        }
    }

    std::string stringifyHistory() {
        std::ostringstream oss;
        for (const auto& [date, value] : pastValues) {
            oss << date << ": " << value;
        };
        return oss.str();
    }
};
