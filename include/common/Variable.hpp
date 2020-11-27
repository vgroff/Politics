#pragma once
#include "./Modifier.hpp"
#include <exception>

template <class T>
class Value {
private:
    std::string valueName;
    T value;
public:
    Value(std::string valueName, T defaultValue)
    : valueName(valueName), value(defaultValue) {};
    std::string getName() const {
        return valueName;
    };
    T getValue() const {
        return value;
    };
    void setValue(T newValue) {
        value = newValue;
    }
};

template <class T>
class Variable {
private:
    std::string variableName;
    Value<T> baseValue;
    Value<T> latestValue;
    std::vector<std::shared_ptr<Modifier<T>>> baseValueModifiers = {};
public:
    Variable(std::string variableName, T defaultValue)
    : variableName(variableName), 
    baseValue("Base value of " + variableName, defaultValue),
    latestValue("Latest value of " + variableName, defaultValue) {}

    void addModifier(std::shared_ptr<Modifier<T>> modifier) {
        baseValueModifiers.push_back(modifier);
    }

    virtual Value<T> getBaseValue() const {
        return baseValue;
    }

    virtual void setBaseValue(T value) {
        baseValue.setValue(value);
    }

    virtual Value<T> calculateValue(bool setLatest) {
        Value<T> value = applyModifiers(baseValue, baseValueModifiers);
        if (setLatest) {
            latestValue = value;
        }
        return value;
    }

    virtual Value<T> getLatest() {
        return latestValue;
    }

    static Value<T> applyModifiers(const Value<T>& baseValue, std::vector<std::shared_ptr<Modifier<T>>> modifiers) {
        std::sort(modifiers.begin(), modifiers.end(), [](std::shared_ptr<Modifier<T>> modifier1, std::shared_ptr<Modifier<T>> modifier2) {
            return modifier1->getPriority() < modifier2->getPriority();
        });
        Value<T> currentValue = baseValue;
        for (auto& modifier : modifiers) {
            currentValue.setValue(modifier->getModification(currentValue)->evaluate());
        }
        return currentValue;
    }

    virtual std::string getDescription() {
        return variableName;
    };
};

class ConstVariableAltered: std::exception {
	const char * what () const throw () {
    	return "ConstVariableAltered";
    }
};

template <class T>
class ConstVariable : Variable<T> {
public:
    ConstVariable(std::string variableName, T defaultValue) 
    : Variable<T>(variableName, defaultValue) {}

    virtual void setCurrentValue(T value) {
        throw ConstVariableAltered();
    }
};

// Perhaps all variables should be functional ones? overhead tho
// template <class T>
// class FunctionalVariable: ConstVariable<T> {
// private:
//     std::function<T()> baseValueGetter; // templatize this?
//     std::vector<Modifier<T>()> modifierGetters;
// public:
//     FunctionalVariable(std::string variableName, std::function<Variable<T>()>  baseValueGetter)
//     : Variable<T>(baseValueGetter()) {}

//     void addModifier(Modifier modifier) {
//         modifiers.push_back(modifier);
//     }

//     virtual Variable getValue() {
//         // TODO: this probably not valid anymore
//         T currentValue = value;
//         for (auto& modifier : modifierGetters) {
//             currentValue = modifier.modify(currentValue);
//         }
//         return currentValue;
//     }
// };

// Still to do:
// - Get it to compile as is
// - Rename to Addition/Multiplication
// - Rename to Evaluate
// - DONT DO THIS WITH TEMPLATES, DO IT WITH INHERITANCE, Add weak pointer (optionally?) to object so that we know where variable came from, could be pointer to mapping that points to object instead 
// - Add soft setting variables
// - Add averaging operation - variadic function - can only average vars of the same type so no variadic class
// - Hysteresis variables
// - Variables that track themselves over time
// - Modifiers that dont have a getModif function - operations are constant
// - Add AND and OR operations
// - Add Negative/Invert unary operations
// - Add divide and exponentiation operations
// - Add + and * operations for the ideology vectors - maybe make this into it's own type?
// - Variables need to save modifications so that they can show what they did in the latest update
// - Computed variables - do we even need these and how do they fit in with the rest?
// - Intermediate "Game" Object that holds the clock and GUI and stuff

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