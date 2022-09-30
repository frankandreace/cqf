#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>     // for std::chrono::high_resolution_clock
#include <string.h>
#include <cassert>
#include <cmath>

#include "filter.hpp" //header in local directory

using namespace std;

#define MEM_UNIT 64
#define MET_UNIT 3
#define OFF_POS 0
#define OCC_POS 1
#define RUN_POS 2

/*
ADDITIONAL METHODS NOT USED BY THE CLASS
*/

uint64_t mask_right(uint64_t numbits){
    uint64_t mask = -(numbits >= MEM_UNIT) | ((1ULL << numbits) - 1);
    return mask;
}

uint64_t get_block(uint64_t position){
    return position / MEM_UNIT;
}

uint64_t get_shift(uint64_t position){
    return position % MEM_UNIT;
}

/*
FILTER CLASS
*/

Cqf::Cqf( uint64_t quotient_s ){
    assert(quotient_s < MEM_UNIT);

    quotient_size = quotient_s;
    remainder_size = MEM_UNIT - quotient_size;
    uint64_t num_bits_quot = 1ULL << quotient_size;
    number_blocks = std::ceil(num_bits_quot/MEM_UNIT);
    uint64_t num_of_words = number_blocks * (MEM_UNIT*(remainder_size+3));

    cqf = vector<uint64_t>(num_of_words);
    m_num_bits = num_of_words*MEM_UNIT;
    
}

uint64_t Cqf::num_bits() const {
        return m_num_bits;
    }

uint64_t Cqf::num_64bit_words() const {
        return cqf.size();
    }

void Cqf::set_word(uint64_t word, uint64_t pos){
    cqf[pos] = word;
}

void Cqf::set_bits(uint64_t pos, uint64_t bits_to_set, uint64_t len) {
    assert(pos + len <= num_bits());
    assert(len == MEM_UNIT or (bits_to_set >> len) == 0);
    if (len == 0) return;

    uint64_t mask = mask_right(len);
    uint64_t block = get_block(pos);
    uint64_t shift = get_shift(pos);

    cqf[block] &= ~(mask << shift);
    cqf[block] |= (bits_to_set << shift);

    uint64_t stored = MEM_UNIT - shift;

    if (len > stored){
        cqf[block+1] &= ~(mask_right(len-stored));
        cqf[block+1] |= (bits_to_set >> stored);
    }
}

vector<uint64_t> Cqf::get_slice(uint64_t pos, uint64_t len) const{
    uint64_t num_int = get_block(len);
    uint64_t rem = get_shift(len);
    vector<uint64_t> slice(num_int+1);

    for(uint64_t j = 0;j < num_int; ++j){
        slice[j] = get_bits(pos+(j*MEM_UNIT),MEM_UNIT);
    }

    slice[num_int] = get_bits(pos+(num_int*MEM_UNIT), rem);
    
    return slice;
}

uint64_t Cqf::get_bits(uint64_t pos, uint64_t len) const {

    assert(pos + len <= num_bits());

    if (!len) return 0;

    uint64_t block = get_block(pos);
    uint64_t shift = get_shift(pos);
    uint64_t mask = mask_right(len);

    if (shift + len <= MEM_UNIT) return (cqf[block] >> shift) & mask;

    return (cqf[block] >> shift) | ((cqf[block+1] << (MEM_UNIT - shift)) & mask);
    //return (cqf[block] >> shift) | ((cqf[block+1] & mask_right(len-(MEM_UNIT-shift))) << (MEM_UNIT - shift));
}

uint64_t Cqf::get_word(uint64_t pos) const{
    assert(pos + MEM_UNIT <= num_bits());
    uint64_t block = get_block(pos);
    uint64_t shift = get_shift(pos);
    uint64_t word = cqf[block] >> shift;

    if (shift > 0 && block+1 < num_64bit_words()){
        word |= (cqf[block+1] << (MEM_UNIT - shift));
    }

    return word;
}

void Cqf::print_slice(uint64_t pos, uint64_t len) const {
    uint64_t num_int = get_block(len);
    uint64_t rem = get_shift(len);
    for(uint64_t j = 0; j < num_int; ++j){
        print_bits(pos+(j*MEM_UNIT),MEM_UNIT);
    }
    print_bits(pos+(num_int*MEM_UNIT), rem);
    cout << endl;
}

void Cqf::print_bits(uint64_t pos, uint64_t len) const {

    uint64_t bits = get_bits(pos,len);
    uint64_t b = bits;
    for (uint64_t j = 0; j < len; ++j){
            cout << (b & 0b1);
        b >>= 1;
    }
    cout << " | ";
    uint8_t* temp = (uint8_t*)&bits;
    for (uint64_t j = 0; j < (len/8)+1; ++j){
        uint8_t byte_num = *temp;
        cout << +byte_num << " ";
        temp++;
    } 
    cout << " | ";
    cout << bits;
    cout << endl;
}

void Cqf::print_word(uint64_t pos) const{
    
    uint64_t word = get_word(pos);
    cout << word << endl;
}








uint64_t Cqf::get_offset(uint64_t position) const{
    assert(position < number_blocks);
    return cqf[position*(3+remainder_size)+OFF_POS];
}

uint64_t Cqf::get_occupieds(uint64_t position) const{
    assert(position < number_blocks);
    return cqf[position*(3+remainder_size)+OCC_POS];
}

uint64_t Cqf::get_runends(uint64_t position) const{
    assert(position < number_blocks);
    return cqf[position*(3+remainder_size)+RUN_POS];
}

uint64_t Cqf::rank(uint64_t num) const{
    return popcnt(num);
}

uint64_t Cqf::select(uint64_t num) const{
    return bitselect(num,rank(num));
}

bool Cqf::contains(uint64_t number) const{
    uint64_t quotient = number & mask_right(quotient_size);

    uint64_t block_id = quotient / MEM_UNIT;
    uint64_t pos_id = quotient % MEM_UNIT;

    if (((get_occupieds(block_id) >> pos_id) & 1ULL) == 0) return false;



    //uint64_t reminder = number >> quotient_size;
    return true;
}

uint64_t bitselect(uint64_t num, uint64_t rank){

    uint64_t i = 1ULL << rank; // i = 2^rank

    // SELECT(v,i) = TZCNT(PDEP(2^rank,num))     
	asm("pdep %[num], %[mask], %[num]"
			: [num] "+r" (num)
			: [mask] "r" (i));

	asm("tzcnt %[bit], %[index]"
			: [index] "=r" (i)
			: [bit] "g" (num)
			: "cc");

	return i;
}


uint64_t popcnt(uint64_t num){
    
    // RANK(v,i) = POPCOUNT(v & (2^(i) -1))
    asm("popcnt %[num], %[num]"
                : [num] "+r" (num)
                :
                : "cc");
        return num;
}


/*
FILTER METHODS


// returns the place where the reminder has been put
uint64_t set_number(uint64_t number, uint64_t qbits){
    uint64_t quotient = number & mask_right(qbits);
    uint64_t remainder = number >>= qbits;
    
    bool was_occupied = set_occupied(quotient);
    uint64_t pos_runend = set_runend(quotient);

    uint64_t set_reminder = set_remainder(pos_runend,remainder);
}

bool set_occupied(uint64_t quotient){

    return false;
}

uint64_t set_runend(uint64_t quotient){

}

uint64_t set_remainder(uint64_t pos_runend, uint64_t remainder){

}
*/