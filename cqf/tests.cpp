#include <iostream>
#include <vector>
#include <string.h>
#include <cassert>
#include "filter.hpp" //header in local directory

#define MEM_UNIT 64

void print_bits_word(uint64_t word){
    for (uint64_t j = 0; j < MEM_UNIT; ++j){
            std::cout << (word & 0b1);
            word >>= 1;
        } 
}

std::vector<uint64_t> shift_vector(std::vector<uint64_t> vec, uint64_t shift){
    
    uint64_t first = vec[0];

    for(uint64_t j = 0; j < vec.size()-1; ++j){
        uint64_t x = vec[j];
        uint64_t y = vec[j+1];

        vec[j] = y >> (MEM_UNIT-shift) | x << shift;
    }
    uint64_t last = vec[vec.size()-1];
    vec[vec.size()-1] = last >> (MEM_UNIT-shift) | first << shift;

    return vec;
}

bool is_correct(uint64_t used, uint64_t retreived) {

    print_bits_word(used);
    std::cout << " ";
    print_bits_word(retreived);
    std::cout << std::endl;
    if (used != retreived){
        std::cout << "expected " << used << " but received " << retreived << std::endl; 
        return false;
    }
    return true;
}

bool test_get_bits(Cqf &filter,std::vector<uint64_t> &num_to_test, uint64_t starting_pos){
    
    for(uint64_t j = 0; j < num_to_test.size(); ++j){
        filter.set_word(num_to_test[j],starting_pos+j);
    }

    for(uint64_t j = 0; j < num_to_test.size(); ++j){
        if (!is_correct(num_to_test[j],filter.get_bits((starting_pos+j)*MEM_UNIT,MEM_UNIT))) return false;
    }
    
    return true;
}

bool test_get_bits_shifted(Cqf &filter,std::vector<uint64_t> &num_to_test, uint64_t starting_pos, uint64_t shift){

    std::vector<uint64_t> shifted_num_to_test = shift_vector(num_to_test, shift);

    for(uint64_t j = 0; j < num_to_test.size(); ++j){
        filter.set_word(shifted_num_to_test[j],starting_pos+j);
    }

    for(uint64_t j = 1; j < num_to_test.size(); ++j){
        if (!is_correct(num_to_test[j],filter.get_bits((starting_pos+j)*MEM_UNIT+shift,MEM_UNIT))) return false;
    }

    uint64_t last = filter.get_bits((starting_pos*MEM_UNIT)+shift,(MEM_UNIT - shift)); //| filter.get_bits((starting_pos+num_to_test.size())*MEM_UNIT+(MEM_UNIT-shift),shift) << shift;
    if (!is_correct(num_to_test[0],last)) return false;
    
    return true;
}

//bool test_set_bits(){}

