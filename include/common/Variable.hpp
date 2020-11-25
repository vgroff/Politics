#pragma once
#include "./Modifier.hpp"

template <class T>
class Variable {
private:
    std::string variableName;
    T baseValue;
    T latestValue;
    std::vector<std::shared_ptr<Modifier<T>>> baseValueModifiers = {};
public:
    Variable(std::string variableName, T defaultValue)
    : variableName(variableName), baseValue(baseValue) {}

    void addModifier(std::shared_ptr<Modifier<T>> modifier) {
        modifiers.push_back(modifier);
    }

    virtual T getBaseValue() const {
        return baseValue;
    }

    virtual void setBaseValue(T value) {
        baseValue = value;
    }

    virtual T calculateValue(bool setLatest) {
        T value = applyModifiers(baseValue, baseValueModifiers);
        if (setLatest) {
            latestValue = value;
        }
        return latestValue;
    }

    virtual T getLatest() {
        return latestValue;
    }

    static T applyModifiers(T baseValue, std::vector<Modifier<T>> modifiers) {
        // Need to order these depending on the type of modifier
        // but it occurs to me that having them be Modifiers is not great,
        // they actually need to be shared pointers in order for us to distinguish
        // the type
        // modifiers->sort(key: priority)
        // for each modifier:
        //     currentValue = modifier.getOperation(currentValue).evaluate()
        for (auto& modifier : modifiers) {
            modifier.modify(currentValue);
        }        
    }

    static virtual std::string getDescription();
};

template <class T>
class ConstVariable : Variable<T> {
public:
    ComplexVariable(T defaultValue) 
    : Variable(defaultValue) {}

    virtual Variable setCurrentValue(T value) {
        throw std::exception("Const variable can't be altered without modifiers");
    }
};

template <class T>
class Evaluator {
private:

public:
    // Give this class the variables you are going to use
    // and provide it with a method that uses these as arguments
    // and returns a T when evaluate() is called i.e. std::function<T(variables)> baseValueGetter
};

// Perhaps all variables should be functional ones?
template <class T>
class FunctionalVariable: ConstVariable<T> {
private:
    std::function<T()> baseValueGetter; // templatize this?
    std::vector<Modifier<T>()> modifierGetters;
public:
    FunctionalVariable(std::string variableName, std::function<Variable<T>()>  baseValueGetter)
    : Variable(baseValueGetter()) {}

    void addModifier(Modifier modifier) {
        modifiers.push_back(modifier);
    }

    virtual Variable getValue() {
        // TODO: this probably not valid anymore
        T currentValue = value;
        for (auto& modifier : modifierGetters) {
            currentValue = modifier.modify(currentValue);
        }
        return currentValue;
    }
};

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