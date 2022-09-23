#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>     // for std::chrono::high_resolution_clock
#include <string.h>

#include "filter.hpp" //header in local directory

using namespace std;

#define MEM_UNIT 64
#define MEM_BLOCK 8

//class Cqf {

    //private:
    //uint64_t * cqf;
    //uint64_t len_cqf;

    //public:
    //Cqf( uint64_t number_of_blocks ); // constructor
    //~Cqf();                           // destructor 



Cqf::Cqf( uint64_t number_of_blocks ){ // constructor

    cqf = new uint64_t[(MEM_UNIT * number_of_blocks)]; 
    memset(cqf, 0, MEM_UNIT * number_of_blocks);
    len_cqf = number_of_blocks;
}

Cqf::~Cqf(){                            // destructor 
    delete[](cqf); // or free?
}

void Cqf::printVector(uint64_t start, uint64_t end){ // printing a slice of the vector
    uint64_t* start_point = cqf + start;
    uint64_t* end_point = cqf + end;
    for(uint64_t* j = start_point; j <= end_point; ++j){
        cout << *j << " ";
    }
    cout << endl;
}

void Cqf::setInt(uint64_t start, uint64_t number){ // printing a slice of the vector
    uint64_t* start_point = cqf + start;
    *start_point = number;
}

void Cqf::setBits(uint64_t start,uint64_t offset, uint64_t length){
        // 1) clear bits at that position
        // 2) mask uint64_t
        // 3) add at position
}

void Cqf::printBits(uint64_t startbit, uint64_t length){
    uint64_t quot = startbit / MEM_BLOCK;
    uint64_t rem = startbit % MEM_BLOCK;
    uint8_t* point = cqf + quot;
    uint8_t b = *point;

    for (uint8_t j = 0; j < 8; ++j){
        if (j >= rem){
            cout << (b & 0b1);
        }
        b >>= 1;
    }
        
}

    /*void printInt(uint64_t start,uint64_t offset, uint64_t length){
        return
    }*/
//};
