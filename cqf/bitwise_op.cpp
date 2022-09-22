#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>     // for std::chrono::high_resolution_clock
#include <string.h>

using namespace std;

#define MEM_UNIT 8

void printBits(uint8_t num){
    bitset<MEM_UNIT> x(num);
    cout << x << '\n';
}

void printxt(char * str){
    cout << str << endl;
}

void setbits(uint8_t * pos, uint8_t value, uint8_t mask)
{
    *pos = (*pos & ~mask);
    *pos += (value & mask);
}


uint8_t * bv(uint64_t number_of_blocks){

    uint8_t * qf = new uint8_t[(MEM_UNIT * number_of_blocks)]; 

    memset(qf, 0, MEM_UNIT * number_of_blocks);

    return qf;
}

void fillcqf(vector<uint8_t> cqf, uint64_t vector_size){
    for (uint64_t i = 0; i != vector_size; ++i){
        cqf[i] = cqf[i] & 0;
        printBits(cqf[i]);
    }

    uint8_t mask1 = (1 << MEM_UNIT) - 1;
    bitset<8> x(mask1);
    cout << x << '\n';
    for (uint64_t i = 0; i != vector_size; ++i){
        cqf[i] = cqf[i] | mask1;
        bitset<8> x(cqf[i]);
        cout << x <<  endl;
    }

}

int main(int argc, char** argv) {
    uint64_t vector_size = 5;

    

    //vector<uint8_t> cqf;
    //cqf.reserve(vector_size);

    //printxt("Reserved");

    //uint8_t * bitvect = bv(vector_size);

    //printxt("bitvector");


    return 0;
}