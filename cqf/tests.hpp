#include <iostream>
#include <vector>
#include <string.h>
#include <cassert>
#include "filter.hpp" //header in local directory

void print_bits_word(uint64_t word);
std::vector<uint64_t> shift_vector(std::vector<uint64_t> vec, uint64_t shift);
bool is_correct(uint64_t used, uint64_t retreived);
bool test_get_bits(Cqf &filter,std::vector<uint64_t> &num_to_test, uint64_t starting_pos);
bool test_get_bits_shifted(Cqf &filter,std::vector<uint64_t> &num_to_test, uint64_t starting_pos, uint64_t shift);