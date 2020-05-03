//  Created by Gabriel Cazaubieilh on 01/02/2020.
#include <iostream>
#include "option.h"

int main(int argc, const char * argv[]) {
    EuropeanCall test(100, 1, 0.2, 0.05);
    std::cout << test.getBlackScholesValue(100) << std::endl;
    KnockOutCall testKnockOut(100, 1, 0.2, 0.05, 3);
    std::cout << testKnockOut.getBlackScholesValue(100) << std::endl;
    // std::cout << test.getValue(100) << std::endl;
    // std::cout << test.getBlackScholesValue(100);
    return 0;
}
