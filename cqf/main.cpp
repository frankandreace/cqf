
#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>     // for std::chrono::high_resolution_clock
#include <string.h>

#include "filter.hpp"

using namespace std;
#define MEM_UNIT 64

void printbits(uint64_t* pos){
    uint64_t b = *pos;
    for(uint64_t j = 0; j < 64; ++j){
        cout << (b & 0b1);
        b >>= 1;
    }
    cout << endl;
}

int main(int argc, char** argv) {


    Cqf cqf(10);

    cout << "created" << endl;

    for(int i = 2; i < 10; ++i){
        cqf.set_bits(i*(MEM_UNIT/4),0b1011111111110011,MEM_UNIT/4);
    }
    
    cqf.set_bits(5*(MEM_UNIT/4),0b1011111111110011,MEM_UNIT/4);

    cqf.set_bits(0,0b1101,4);
    cqf.set_bits(5,54,8);
    cqf.print_slice(0,5);
    cqf.print_bits(0,16);
    cqf.print_slice(0,16);
    cqf.print_word(15);
    cout << "15 to 192" << endl;
    cqf.print_slice(15,160);

    cqf.print_bits(15,64);
    cqf.print_bits(79,64);
    cqf.print_bits(143,49);

    cout << "0 to 192" << endl;

    cqf.print_slice(0,192);

    cqf.print_bits(0,64);
    cqf.print_bits(64,64);
    cqf.print_bits(128,64);

    
    return 0;
}