#pragma once
#include "./Modifier.hpp"
#include "./Value.hpp"
#include <exception>
#include <memory>

template <class T>
class Variable {
protected:
    std::string name;
    Value<T> baseValue;
    TrackedValue<T> latestValue;
    std::vector<std::shared_ptr<Modifier<T>>> baseValueModifiers = {};
    Calculation<T> latestCalculation;
public:
    Variable(std::string name, 
            T defaultValue, 
            std::weak_ptr<Clock> clock,
            size_t trackingRateDays = 0)
    : name(name), 
    baseValue("Base value of " + name, defaultValue, this),
    latestValue("Latest value of " + name, defaultValue, this, clock, trackingRateDays),
    latestCalculation("Latest calculation of " + name) {};

    Variable(std::string name, 
            T defaultValue, 
            std::weak_ptr<Clock> clock,
            std::pair<T,T> bounds_, 
            size_t trackingRateDays = 0)
    : name(name), 
    baseValue("Base value of " + name, defaultValue, bounds_, this),
    latestValue("Latest value of " + name, defaultValue, bounds_, this, clock, trackingRateDays),
    latestCalculation("Latest calculation of " + name) {};

    ~Variable() {
        baseValue.setParent(nullptr);
        latestValue.setParent(nullptr);
    }

    void addModifier(std::shared_ptr<Modifier<T>> modifier) {
        baseValueModifiers.push_back(modifier);
    }

    virtual TrackedValue<T> getLatest() const {
        return latestValue;
    }

    Calculation<T> calculate(bool setLatest) {
        Calculation<T> calculation("Calculation for " + name, baseValue, baseValueModifiers);
        if (setLatest) {
            T result = calculation.getResult();
            latestValue.softSetValue(result);
            latestCalculation = calculation;
        }
        return calculation;
    }

    Calculation<T> getLatestCalculation() const {
        return latestCalculation;
    }

    virtual std::string getDescription() const {
        return name;
    };
};

template <class T>
class HysteresisVariable: public Variable<T> {
public: 
    Variable<T> targetVariable;
    Variable<double> growthRateVar;
    std::vector<std::shared_ptr<Modifier<T>>> currentValueModifiers = {};
    std::vector<std::shared_ptr<Modifier<T>>> growthRateModifiers = {};

    HysteresisVariable(std::string name, 
            T defaultValue, 
            std::weak_ptr<Clock> clock,
            size_t trackingRateDays = 0)
    : Variable<T>(name, defaultValue, clock, trackingRateDays),
    targetVariable("Target variables of " + name, defaultValue, std::weak_ptr<Clock>(), trackingRateDays),
    growthRateVar("Growth rate of " + name, 0, std::weak_ptr<Clock>(), {0, 1}, trackingRateDays)
    {};

    HysteresisVariable(std::string name, 
            T defaultValue, 
            std::weak_ptr<Clock> clock,
            std::pair<T,T> bounds_, 
            size_t trackingRateDays = 0)
    : Variable<T>(name, defaultValue, clock, bounds_, trackingRateDays),
    targetVariable("Target variables of " + name, defaultValue, std::weak_ptr<Clock>(), bounds_, trackingRateDays),
    growthRateVar("Growth rate of " + name, 0, std::weak_ptr<Clock>(), {0, 1}, trackingRateDays)
    {};

    Calculation<T> calculate(bool setLatest) {
        auto currentVar = Variable<T>::calculate(setLatest);
        if (setLatest) {
            currentValueModifiers = {};
        }
        targetVariable.calculate(setLatest);
        auto growthRateCalc = growthRateVar.calculate(setLatest);
        Value<double> currentValue("Calculated current value of " + this->name, currentVar->getResult(), this);
        auto newValue = std::make_shared<Multiplication<double, T>>(growthRateVar, currentValue);
        Calculation<T> calculation("Value of " + this->name + " including growth", {newValue});
        if (setLatest) {
            currentValue.softSetValue(calculation.getResult());
        }
        return calculation;
    }
};

// Still to do:
// - Have activating modifiers - need boolean operations
// - Convert existing code to using Variables!!! Have a big init method that sets everything up then
//      - We'll want to calculate some values in order rather than having them rely on potentially old values
//         which begs the question of when exactly values should update - on command, or on the clock?
//          On the clock might be "nicer" since it is automatic, we just need to subscribe those objects to the clock at the correct time
//          which just means creating them at the correct time on init
// - Hysteresis modifiers! Easy or not? Could just be done through intermediary variables but might be ugly? Otherwise hysteresis on the value itself shouldn't be too hard
// - Functional variables ?
// - Need a ModificationPriority since some modifiers may return varied outputs
// - Hysteresis variables that grow and fall differently
// - DONT DO THIS WITH TEMPLATES, DO IT WITH INHERITANCE, Add weak pointer (optionally?) to object so that we know where variable came from, could be pointer to mapping that points to object instead 
// - Add averaging operation - variadic function - can only average vars of the same type so no variadic class
// - Modifiers that dont have a getModif function - operations are constant ?
// - Add AND and OR operations
// - Add Negative/Invert unary operations
// - Add divide and exponentiation operations
// - Add + and * operations for the ideology vectors - maybe make this into it's own type?
// - Computed variables - do we even need these and how do they fit in with the rest?
// -

// template <class T, class U>
// class ComplexVariable : Variable<T> {
// private:
//     std::vector<Modifier> modifiers;
//     std::weak_ptr<U> contextPtr;
// public:
//     ComplexVariable(T defaultValue, std::weak_ptr<U> contextPtr) 
//     : Variable(defaultValue), contextPtr(contextPtr) {}
// };

// What should have access to higher objects and when?
// If we want variables to calculate themselves, they need to have access to 
// other variables, which they should do via the higher objects in order to set a base value
// The rest should be done only by modifiers, that can themselves be set by ComplexVariable
// so that they remain explainable
// Or we have this thing where variables/complexVariables have to declare what other variables
// they would like to use
// OR modifiers have to call back to a mother function, and we can eliminate contextPtr entriely
// If modifiers somehow declare the variables they want, they can also be updated if or when these variables change 

// Let's try out some code:
// What it is before:
// double productivityDiff = (researchProps.research - privateIndustry.getProductivity())/researchProps.research;
// researchProps.research = researchProps.research*(1 + 0.01*MONTHS_PER_TURN/12 + 0.015*std::pow(productivityDiff, 2)*MONTHS_PER_TURN/12);
// privateIndustry.setCurrentTechnology(researchProps.research);
// Now:
// weak_self = from_self()
// productivityVariable = Variable<double>(defaultProductivity)
// productivityModifier = MultiplicativeModifier<double>([weak_self](const Variable<double> baseValue, const Variable<double> currentValue) {
//     weak_self->nationVariables[Research].getCurrentValue();
//     weak_self->globalVariables[MonthsPerTurn].getCurrentValue();
//     weak_self->privateIndustry.getVariable(Productivity).getCurrentValue();
//     // do the calculations
//     return MultiplicativeModifier(value);
// });
// variables[EconVarProductivity] = productivityVariable
// Have explainable modifiers? e.g. output the variables used? Have a function that creates the modifier and attaches the variables it can access in a vector<Variable>
// 
// POTENTIAL PROBLEM:
// Consider: 
// - Bullingdon Boy modifier, gives opinion boost to politicians that are also Bullingdon Boys
// - Has to apply to all politicians but only activate conditionally (if they are also Bullingdon Boys)
// - I.e. has to take the current politician as an argument - how do we do that?
// Example code:
// Setting the modifier:
// politician1 = Politician
// politician1.addTrait(Trait("BullingdonBoy", {
//   this.name = "BullingdonBoy"
//   politician1->addGlobalOpinionModifier((politician2) {
//       variables[0] = politician2->traits["BullingdonBoy"]
//       return (variables) {
//          return isActive(variables[0]);
//   }, {
//       return AdditiveModifier(0.25);
//   }));
// }
// Reading the modifier:
// ?
// i.e. we need to be able to have modifiers that are generated using specific Politician objects
// Here the modifier is applied to all other politicians, either all of them liking us or us liking all of them
// So when politician2 is evaluating their opinion of politician1, they will gather all of politcian1s global opinion modifiers, evaluate them
// and pass them to value as extra modifiers! e.g.
// politician2 = Poltician
// opinionPol1 = Value("Opinion politican1.name -> politician2.name", 0)
// Value.modify([politician2](
//    listOfModifiers = politician1.getAllModifiers().evaluateAll()
//    return listOfModifiers;
// )) -> modify would then use this list as well
// politcian2.addOpinion(opinionPol1)