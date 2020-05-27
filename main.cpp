#include <iostream>
#include "option.h"

int main(int argc, const char * argv[]) {
    // Examples
    ExtendibleCall exted(73, 0.25, 0.25, 0.02);
    std::cout << "Extendible Call: " << exted.getBinomialTreeValue(75, 1000) << std::endl;
    EuropeanCall europCall(1470.37, 0.5, 0.12, 0.02);
    std::cout << "European Call: " << europCall.getBlackScholesValue(1400) << std::endl;
    EuropeanPut europPut(63.75, 1, 0.2, 0.04);
    double val = europPut.getBlackScholesValue(75);
    std::cout << "European put: " << val << std::endl;

    CompoundEuropeanCall compEurCall(2.5, 3.0 / 12, 0.25, 0.01);
    std::cout << "Compound Call" << compEurCall.getBinomialTreeValue(80, 30) << std::endl;

    ReloadableCallOption reloadCall(110, 2, 0.32, 0.0195);
    std::cout << " Price of reloadable call with initial price 100: " << reloadCall.getBinomialTreeValue(100, 200);

    return 0;
}
