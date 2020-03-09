#include "../../include/electors/Utility.hpp"

double getUtility(double income) {
    return 1 - 1/(1+(income*2));
}
// y = 1 - 1/(1+2x) => 1/(1-y) = 1+2x => 1/2*(1/(1-y) - 1)
double inverseUtility(double utility) {
    return 0.5*(1.0/(1-utility) - 1);
}