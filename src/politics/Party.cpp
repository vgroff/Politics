#include "../../include/politics/Party.hpp"


Party::Party(std::string name, PoliticalCompassPoint currentIdeology)
    : name(name),
    currentIdeology(currentIdeology) {
    
}

PoliticalCompassPoint Party::getCurrentIdeology() const {
    return currentIdeology;
}

std::string Party::getName() const {
    return name;
}