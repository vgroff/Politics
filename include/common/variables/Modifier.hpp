#pragma once
#include "./Operation.hpp"
#include "./UnaryOperation.hpp"
#include "./BinaryOperation.hpp"


enum ModifierPriority {
    Setter,
    Linear,
    Multiplicative
};

template <class T>
class Modifier {
private:
    std::string description = "";
    ModifierPriority priority;
public:
    Modifier(std::string modifierDescription, ModifierPriority priority)
    : description(modifierDescription), priority(priority) {};

    ModifierPriority getPriority() {
        return priority;
    }
    std::shared_ptr<Operation<T>> virtual getModification(Value<T> variable) = 0;
};

template <class T>
class AdditiveModifier: public Modifier<T> {
private:
    std::function<std::shared_ptr<Addition<T>>(Value<T>)> getModif;
public:
    AdditiveModifier(std::string modifierDescription, 
            ModifierPriority priority,
            std::function<std::shared_ptr<Addition<T>>(Value<T>)> getModif)
    : Modifier<T>(modifierDescription, priority), getModif(getModif) {};
    AdditiveModifier(std::string modifierDescription, 
            std::function<std::shared_ptr<Addition<T>>(Value<T>)> getModif)
    : Modifier<T>(modifierDescription, Linear), getModif(getModif) {};

    std::shared_ptr<Operation<T>> getModification(Value<T> variable) {
        return getModif(variable);
    };
};

template <class T, class U>
class MultiplicativeModifier: Modifier<T> {
private:
    std::function<std::shared_ptr<Multiplication<T, U>>(Value<T>)> getModif;
public:
    MultiplicativeModifier(std::string modifierDescription, 
            ModifierPriority priority,
            std::function<std::shared_ptr<Multiplication<T, U>>(Value<T>)> getModif)
    : Modifier<T>(modifierDescription, priority), getModif(getModif) {};
    MultiplicativeModifier(std::string modifierDescription, 
            std::function<std::shared_ptr<Multiplication<T, U>>(Value<T>)> getModif)
    : Modifier<T>(modifierDescription, Multiplicative, getModif) {};

    std::shared_ptr<Operation<T>> getModification(Value<T> variable) {
        return getModif(variable);
    };
};

template <class T>
class CustomModifier: Modifier<T> {
private:
    std::function<std::shared_ptr<Operation<T>>(std::shared_ptr<Value<T>>)> getModif;
public:
    CustomModifier(std::string modifierDescription, 
            ModifierPriority priority, 
            std::function<std::shared_ptr<Operation<T>>(Value<T>)> getModif)
    : Modifier<T>(modifierDescription, priority), getModif(getModif) {};

    std::shared_ptr<Operation<T>> getModification(Value<T> variable) {
        return getModif(variable);
    };
};
