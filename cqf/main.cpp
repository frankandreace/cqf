
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


    /*
    uint64_t len = 70;

    uint64_t mask = ((1ULL << len) - 1);

    uint64_t mask_el1 = (len == 64);
    uint64_t mask_el11 = -(len == 64);
    uint64_t mask_el2 = ((1ULL << len) - 1);
    cout << "(len==64): " << mask_el1 << endl;
    printbits(&mask_el1);
    cout << "-(len==64): " << mask_el11 << endl;
    printbits(&mask_el11);
    cout << "((1ULL << len) - 1): " << mask_el2 << endl;
    printbits(&mask_el2);
    cout << "mask value: " << mask << endl;
    uint64_t* mask_p = &mask;
    printbits(mask_p);

    cqf.setInt(0,0b11110001000011100011111010010101110011111);
    cqf.setInt(1,2070652201884L);

    for(int i = 2; i < 10; ++i){
        cqf.setInt(i,3*i);
    }
    cout << "printvector" << endl;

    cqf.printVector(0,9);

    cout << "printvector" << endl;

    cqf.printVector(1,2);

    cout << "print" << endl;

    cqf.printBits(0,64);

    cout << "print" << endl;

    cqf.printBits(64,64);

    cout << "range" << endl;

    cqf.printRangeBits( point,109,12);
    */
    return 0;
}