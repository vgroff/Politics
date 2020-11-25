#pragma once

template <class T>
class Variable;

template <class T>
class Operation;

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

    std::shared_ptr<Operation<T>> getModification(std::shared_ptr<Variable<T>> variable) = 0;
};

template <class T>
class AdditiveModifier: Modifier<T> {
private:
    std::function<std::shared_ptr<AdditiveOperation<T>>(std::shared_ptr<Variable<T>>)> getModif;
public:
    Modifier(std::string modifierDescription, 
            ModifierPriority priority,
            std::function<std::shared_ptr<AdditiveOperation<T>>(std::shared_ptr<Variable<T>>)> getModif)
    : description(modifierDescription), priority(priority), getModif(getModif) {};
    Modifier(std::string modifierDescription, 
            std::function<std::shared_ptr<AdditiveOperation<T>>(std::shared_ptr<Variable<T>>)> getModif)
    : Modifier(modifierDescription, Linear, getModif) {};

    std::shared_ptr<Operation<T>> getModification(std::shared_ptr<Variable<T>> variable) {
        return getModif(variable);
    };
};

template <class T>
class MultiplicativeModifier: Modifier<T> {
private:
    std::function<std::shared_ptr<MultiplicativeOperation<T>>(std::shared_ptr<Variable<T>>)> getModif;
public:
    Modifier(std::string modifierDescription, 
            ModifierPriority priority,
            std::function<std::shared_ptr<MultiplicativeOperation<T>>(std::shared_ptr<Variable<T>>)> getModif)
    : description(modifierDescription), priority(priority), getModif(getModif) {};
    Modifier(std::string modifierDescription, 
            std::function<std::shared_ptr<MultiplicativeOperation<T>>(std::shared_ptr<Variable<T>>)> getModif)
    : Modifier(modifierDescription, Multiplicative, getModif)

    std::shared_ptr<Operation<T>> getModification(std::shared_ptr<Variable<T>> variable) {
        return getModif(variable);
    };
};

template <class T>
class CustomModifier: Modifier {
private:
    std::function<std::shared_ptr<Operation<T>>(std::shared_ptr<Variable<T>>)> getModif;
public:
    Modifier(std::string modifierDescription, 
            ModifierPriority priority, 
            std::function<std::shared_ptr<Operation<T>>(Variable<T>)> getModif)
    : description(modifierDescription), priority(priority), getModif(getModif) {};

    std::shared_ptr<Operation<T>> getModification(std::shared_ptr<Variable<T>> variable) {
        return getModif(variable);
    };
};
