#include "../../include/electors/Utility.hpp"

double getUtility(double income) {
    return 1 - 1/(1+(income*2));
}