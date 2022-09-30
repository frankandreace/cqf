
#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>     // for std::chrono::high_resolution_clock
#include <string.h>

#include "filter.hpp"
#include "tests.hpp"

#define MEM_UNIT 64

void printbits(uint64_t* pos){
    uint64_t b = *pos;
    for(uint64_t j = 0; j < 64; ++j){
        std::cout << (b & 0b1);
        b >>= 1;
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    uint64_t filt_len = 10;
    uint64_t num_el_test_vector = 20;

    Cqf cqf(filt_len);

    std::cout << "created" << std::endl;
    /*
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
    std::cout << "15 to 192" << std::endl;
    cqf.print_slice(15,160);

    cqf.print_bits(15,64);
    cqf.print_bits(79,64);
    cqf.print_bits(143,49);

    std::cout << "0 to 192" << std::endl;

    cqf.print_slice(0,192);

    cqf.print_bits(0,64);
    cqf.print_bits(64,64);
    cqf.print_bits(128,64);
    */

    std::vector<uint64_t> test_vector(num_el_test_vector);

    for(uint64_t j = 0; j < num_el_test_vector; ++j ){
        test_vector[j] = 0b1011011101111;
    }

    bool test1 = test_get_bits(cqf,test_vector,2);

    std::cout << std::endl;
    if (test1) std::cout << "test 1 passed." << std::endl;
    std::cout << std::endl;

    std::cout << std::endl;

    bool test2 = test_get_bits_shifted(cqf,test_vector,23,11);

    std::cout << std::endl;
    if (test2) std::cout << "test 2 passed." << std::endl;
    
    cqf.print_slice(0,MEM_UNIT*45);

    cqf.set_bits(14,0b10111110011,11);

    cqf.print_slice(0,MEM_UNIT*45);

    cqf.set_bits(0,1523,11);
    cqf.print_slice(0,MEM_UNIT*45);

    cqf.print_slice(0,30);
    std::cout << cqf.rank(cqf.get_bits(0,30)) << " " << cqf.select(cqf.get_bits(0,30)) << std::endl;
    cqf.print_slice(2,30);
    std::cout << cqf.rank(cqf.get_bits(2,30)) << " " << cqf.select(cqf.get_bits(2,30)) << std::endl;
    cqf.print_slice(12,30);
    std::cout << cqf.rank(cqf.get_bits(12,30)) << " " << cqf.select(cqf.get_bits(12,30)) << std::endl;
    cqf.print_slice(16,30);
    std::cout << cqf.rank(cqf.get_bits(16,30)) << " " << cqf.select(cqf.get_bits(16,30)) << std::endl;
    return 0;
}