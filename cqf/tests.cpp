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

uint64_t test_rank(uint64_t word, uint64_t position){

    return bitrank(word, position);
}

uint64_t test_select(uint64_t word, uint64_t rank){
    return bitselect(word, rank);
}

bool check_rank_result(uint64_t word, uint64_t position){
    assert(position < MEM_UNIT);
    uint64_t result = bitrank(word, position);
    uint64_t count = 0;
    uint64_t check_counter = 0;
    
    std::cout << word << std::endl;
    print_bits_word(word);

    for(uint64_t i = 0; i <= position; ++i){
        
        count += (word & 1ULL);
        word >>= 1;
        ++check_counter;
    }
    
    if(result == count) return true;
    else{ 
        std::cout << "COUNTER FOR LOOP " << check_counter << std::endl;
        std::cout << std::endl <<  "RANK = " << result  <<  " while check is = " << count << " || with position = " << position << std::endl;

        return false;
        }
}

bool check_select_result(uint64_t word, uint64_t rank){

    uint64_t result = bitselect(word, rank);
    uint64_t position = 0;
    uint64_t count = 0;

    uint64_t num_ones = rank + 1;

    std::cout << word << std::endl;
    print_bits_word(word);

    while(position < MEM_UNIT){
        count += (word & 1ULL);
        if(count == num_ones) break;
        word >>= 1;
        ++position;
    }

    if(result == position) return true;
    else{ 
        std::cout << std::endl <<  "select = " << result  <<  " while check is = " << position << " || with rank = " << rank << std::endl;
        std::cout << "end" << std::endl;

        return false;
        }
    }

void test_rank_and_select(uint64_t num_el_test_vector){

    std::vector<uint64_t> test_vector(num_el_test_vector);

    for(uint64_t j = 0; j < num_el_test_vector-1; ++j ){
        test_vector[j] = 1ULL << j;
    }

    test_vector[64]=0b1111111111111111111111111111111111111111111111111111111111111111;
    test_vector[65]=0b0000000000000000000000000000000000000000000000000000000000000000;
    test_vector[66]=0b1100010000111111111000001111000101110110111111111110001111111011;
    test_vector[67]=0b0001111111110000011000000000011000100100000001011010000000011001;
    test_vector[68]=0b1011011101111110111100111111110111111100011111100101011111110110;
    test_vector[69]=0b1000100001111000110001001001111100001010000000000011110000000000;

    //test_vector[1]=0b00000000000000000000000000000000000000000000000000000000000000000;
    //test_vector[1]=0b0111110000101000000000001111000000000010001000011110001100010010;
    

    
    uint64_t n_false_rank = 0;
    uint64_t n_false_select = 0;
    uint64_t checks_done = 0;
    for(uint64_t j = 0; j < num_el_test_vector; ++j ){
        for(uint64_t i = 0; i < MEM_UNIT; ++i ){

            bool x = check_rank_result(test_vector[j],i);
            bool y = check_select_result(test_vector[j],i);

            if (x == false) n_false_rank++;
            if (y == false) n_false_select++;
            checks_done++;
        }
    }
    
    std::cout << "RANK ERRORS: " << n_false_rank << std::endl;
    std::cout << "SELECT ERRORS: " << n_false_select << std::endl;

    std::cout << "RANK ERRORS PERCENTAGE: " << (n_false_rank / checks_done) << std::endl;
    std::cout << "SELECT ERRORS PERCENTAGE: " << (n_false_select / checks_done) << std::endl;
}