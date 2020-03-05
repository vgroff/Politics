#include "../../include/electors/Elector.hpp"

std::string workerTypeToString(WorkerType workerType) {
    if (workerType == Unskilled) {
        return "Unskilled";
    } else if (workerType == Skilled) {
        return "Skilled";
    } else if (workerType == HighSkilled) {
        return "High Skilled";
    }
    return "Unknown Worker Type";
}