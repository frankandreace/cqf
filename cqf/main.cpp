
#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>     // for std::chrono::high_resolution_clock
#include <string.h>

#include "filter.hpp"

using namespace std;

int main(int argc, char** argv) {


    Cqf cqf(10);

    cout << "created" << endl;

    for(int i = 0; i < 10; ++i){
        cqf.setInt(i,10*i);
    }
    cout << "print" << endl;

    cqf.printVector(0,9);

    cout << "print" << endl;

    cqf.printVector(1,2);

    return 0;
}