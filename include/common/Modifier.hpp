#pragma once
#include "./Operation.hpp"

enum ModifierPriority {
    First,
    Linear,
    Multiplicative,
    Last
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
    std::shared_ptr<Operation<T>> virtual getModification(T variable) = 0;
};

template <class T>
class AdditiveModifier: public Modifier<T> {
private:
    std::function<std::shared_ptr<AdditiveOperation<T>>(std::shared_ptr<Variable<T>>)> getModif;
public:
    AdditiveModifier(std::string modifierDescription, 
            ModifierPriority priority,
            std::function<std::shared_ptr<AdditiveOperation<T>>(std::shared_ptr<Variable<T>>)> getModif)
    : Modifier<T>(modifierDescription, priority), getModif(getModif) {};
    AdditiveModifier(std::string modifierDescription, 
            std::function<std::shared_ptr<AdditiveOperation<T>>(std::shared_ptr<Variable<T>>)> getModif)
    : Modifier<T>(modifierDescription, Linear), getModif(getModif) {};

    std::shared_ptr<Operation<T>> getModification(T variable) {
        return getModif(variable);
    };
};

template <class T>
class MultiplicativeModifier: Modifier<T> {
private:
    std::function<std::shared_ptr<MultiplicativeOperation<T>>(std::shared_ptr<Variable<T>>)> getModif;
public:
    MultiplicativeModifier(std::string modifierDescription, 
            ModifierPriority priority,
            std::function<std::shared_ptr<MultiplicativeOperation<T>>(std::shared_ptr<Variable<T>>)> getModif)
    : Modifier<T>(modifierDescription, priority), getModif(getModif) {};
    MultiplicativeModifier(std::string modifierDescription, 
            std::function<std::shared_ptr<MultiplicativeOperation<T>>(std::shared_ptr<Variable<T>>)> getModif)
    : Modifier<T>(modifierDescription, Multiplicative, getModif) {};

    std::shared_ptr<Operation<T>> getModification(T variable) {
        return getModif(variable);
    };
};

template <class T>
class CustomModifier: Modifier<T> {
private:
    std::function<std::shared_ptr<Operation<T>>(std::shared_ptr<Variable<T>>)> getModif;
public:
    CustomModifier(std::string modifierDescription, 
            ModifierPriority priority, 
            std::function<std::shared_ptr<Operation<T>>(Variable<T>)> getModif)
    : Modifier<T>(modifierDescription, priority), getModif(getModif) {};

    std::shared_ptr<Operation<T>> getModification(T variable) {
        return getModif(variable);
    };
};
