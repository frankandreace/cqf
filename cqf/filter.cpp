#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>     // for std::chrono::high_resolution_clock
#include <string.h>
#include <cassert>
#include <cmath>

#include "filter.hpp" //header in local directory

using namespace std;


// STATIC VARIABLES 
#define MEM_UNIT 64
#define MET_UNIT 3
#define OFF_POS 0
#define OCC_POS 1
#define RUN_POS 2


/*
    
OPERATIONS ON THE ENTIRE CQF
    
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

void Cqf::resize(uint64_t new_size){}

void Cqf::load(){}

void Cqf::save(){}

/*

INFORMATION PROVIDING METHODS

*/

uint64_t Cqf::num_bits() const {
        return m_num_bits;
    }

uint64_t Cqf::num_64bit_words() const {
        return cqf.size();
    }

 uint64_t Cqf::num_reminders_in_filter() const{}

/*

HIGH LEVEL OPERATIONS

*/


uint64_t Cqf::insert(uint64_t number){
    //get quotient q and remainder r
    uint64_t q = quotient(number);
    uint64_t r = remainder(number);

    //get the block number and position in the block of the quotient
    uint64_t block = get_block(q);
    uint64_t pos_in_block = get_shift(q);

    //    to complete ...
}


uint64_t Cqf::query(uint64_t number) const{
    //get quotient q and remainder r
    uint64_t q = quotient(number);
    uint64_t r = remainder(number);

    if (is_occupied(q) == false) return 0;

    //get offset from q
    uint64_t offset = get_offset(q); // TODO

    //get runend of quotient
    uint64_t runend_pos = q + offset;

    uint64_t runend = 0;
    while((runend_pos >= q) && (runend ==0)){

        uint64_t remainder_in_filter = get_remainder(runend_pos); 
        if (remainder_in_filter == r) return 1;
        --runend_pos;
        runend = runend_value(runend_pos);
    }
    return 0;
}

/*
    //get runend of quotient
    uint64_t runend_pos = 0;
    uint64_t runend_block = block;
    if ((pos_in_block+offset) > MEM_UNIT) {
        runend_pos =  (pos_in_block+offset) % MEM_UNIT;
        ++runend_block;
    }
    else runend_pos = pos_in_block + offset;
*/


uint64_t Cqf::remove(uint64_t number){
    //get quotient q and remainder r
    uint64_t q = quotient(number);
    uint64_t r = remainder(number);

}


/*

MID LEVEL OPERATIONS

*/


uint64_t Cqf::get_remainder(uint64_t position) const{

    int64_t block = get_block(position);
    uint64_t pos_in_block = get_shift(position);

    uint64_t pos = (block*((3+remainder_size)*MEM_UNIT)+3*MEM_UNIT+pos_in_block*remainder_size);

    return get_bits(pos, remainder_size);
}


uint64_t Cqf::set_remainder(uint64_t position, uint64_t value){
    int64_t block = get_block(position);
    uint64_t pos_in_block = get_shift(position);

    uint64_t pos = (block*((3+remainder_size)*MEM_UNIT)+3*MEM_UNIT+pos_in_block*remainder_size);

    set_bits(pos, value, remainder_size);

}


uint64_t Cqf::remove_remainder(uint64_t position){
    int64_t block = get_block(position);
    uint64_t pos_in_block = get_shift(position);

    uint64_t pos = (block*((3+remainder_size)*MEM_UNIT)+3*MEM_UNIT+pos_in_block*remainder_size);

    set_bits(pos, 0ULL,remainder_size);
}


uint64_t Cqf::runend_value(uint64_t position) const{
    
    uint64_t block = get_block(position);
    uint64_t pos_in_block = get_shift(position);

    return get_bit_from_word(get_runends(block) ,pos_in_block);
}

bool Cqf::is_occupied(uint64_t position) const{
    
    uint64_t block = get_block(position);
    uint64_t pos_in_block = get_shift(position);

    return get_bit_from_word(get_occupieds(block) ,pos_in_block);
}

uint64_t Cqf::first_unused_slot(uint64_t position) const{}


uint64_t Cqf::runend_pos(uint64_t position) const{}


uint64_t Cqf::get_offset(uint64_t block, uint64_t pos_in_block) const{}


void Cqf::set_offset(uint64_t value, uint64_t block_num){
    assert(block_num < number_blocks);
    /* as of now, the offset it store in a uint64_t (less efficient in terms of memory)
    if I want to set the offset of block number 5, its word position in the uint64_t vector is going to be
    blocK_num (=5) * size_of_the_block (in terms of uint64_t words) + the offset of the offset word in the block (=0)
    */
    uint64_t offset_position = block_num * (remainder_size+3) + OFF_POS; 
    set_word(value, offset_position);
}



/*

BLOCK LEVEL OPERATIONS

*/


uint64_t Cqf::get_reminder_block(uint64_t slot) const{}


uint64_t Cqf::set_reminder_block(uint64_t slot){}


uint64_t Cqf::remove_reminder_block(uint64_t slot){}


bool Cqf::is_occupied_block(uint64_t position) const{}


uint64_t Cqf::runend_pos_block(uint64_t position) const{}


/*

SMALL LEVEL OPERATIONS

*/


uint64_t Cqf::rank(uint64_t value){}


uint64_t Cqf::select(uint64_t value){} 


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

uint64_t Cqf::get_bit_from_word(uint64_t word_pos_in_cqf, uint64_t pos_bit) const{
    assert(word_pos_in_cqf < cqf.size());
    return ((cqf[word_pos_in_cqf] >> pos_bit) & 0b1);
}


/*

PRINTING + DEBUGGING STUFF (SOME OF THEM SHOULD BE MOVED TO PRIVATE OR ELIMINATED)

*/

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
    assert(pos < number_blocks);
    uint64_t word = get_word(pos);
    cout << word << endl;
}


uint64_t Cqf::get_offset(uint64_t position) const{
    assert(position < number_blocks);
    return cqf[position*(MET_UNIT+remainder_size)+OFF_POS];
}


uint64_t Cqf::get_occupieds(uint64_t position) const{
    assert(position < number_blocks);
    return cqf[position*(MET_UNIT+remainder_size)+OCC_POS];
}


uint64_t Cqf::get_runends(uint64_t position) const{
    assert(position < number_blocks);
    return cqf[position*(MET_UNIT+remainder_size)+RUN_POS];
}


bool Cqf::contains(uint64_t number) const{
    uint64_t quotient = number & mask_right(quotient_size);

    uint64_t block_id = quotient / MEM_UNIT;
    uint64_t pos_id = quotient % MEM_UNIT;

    if (((get_occupieds(block_id) >> pos_id) & 1ULL) == 0) return false;

    //uint64_t reminder = number >> quotient_size;
    
    //uint64_t l = bitselect(get_occupieds(block_id));
    return true;
}


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

uint64_t Cqf::quotient(uint64_t num) const{
    return num & mask_right(quotient_size);
}

uint64_t Cqf::remainder(uint64_t num) const{
    return num >> (MEM_UNIT - remainder_size);
}

/*
ROB PATRO'S PAPER IMPLEMENTATIONS
*/

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


uint64_t bitrank(uint64_t val, uint64_t pos) {
	val = val & ((2ULL << pos) - 1);
	asm("popcnt %[val], %[val]"
			: [val] "+r" (val)
			:
			: "cc");
	return val;
}

