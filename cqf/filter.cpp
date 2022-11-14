#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>     // for std::chrono::high_resolution_clock
#include <string.h>
#include <cassert>
#include <cmath>

#include "filter.hpp" //header in local directory

//using namespace std;


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
    block_size = remainder_size + MET_UNIT;
    uint64_t num_of_words = number_blocks * (MEM_UNIT * block_size);

    cqf = std::vector<uint64_t>(num_of_words);
    m_num_bits = num_of_words*MEM_UNIT;
}

Cqf::Cqf(uint64_t quotient_s, uint64_t n_blocks){
    assert(quotient_s < MEM_UNIT);

    quotient_size = quotient_s;
    remainder_size = MEM_UNIT - quotient_size;
    block_size = remainder_size + MET_UNIT;
    number_blocks = n_blocks;
    uint64_t num_of_words = number_blocks * (MEM_UNIT * block_size);

    cqf = std::vector<uint64_t>(num_of_words);
    m_num_bits = num_of_words*MEM_UNIT;
}

//void Cqf::resize(uint64_t new_size){}

//void Cqf::load(){}

//void Cqf::save(){}

/*

INFORMATION PROVIDING METHODS

*/

uint64_t Cqf::num_bits() const {
        return m_num_bits;
    }

uint64_t Cqf::num_64bit_words() const {
        return cqf.size();
    }

 uint64_t Cqf::num_reminders_in_filter() const{
    return 0;
 }

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

    // TODO:
    // THIS SHOULD BE

    // GET FIRST UNUSED SLOT

    // GET POSITION WHERE TO INSERT TO (BASED ON VALUE) IN THE RUN (INCREASING ORDER)

    // SHIFT EVERYTHING RIGHT

    //INSERT ON POSITION
}


uint64_t Cqf::query(uint64_t number) const{
    //get quotient q and remainder r
    uint64_t quot = quotient(number);
    uint64_t rem = remainder(number);

    if (is_occupied(quot) == false) return 0;

    //get runend of quotient
    uint64_t runend_pos = get_runend_pos(quot);

    // TODO:
    // THIS SHOULD BE BINARY SEARCH

    uint64_t runend = 0;
    while((runend_pos >= quot) && (runend == 0)){

        uint64_t remainder_in_filter = get_remainder(runend_pos); 
        if (remainder_in_filter == rem) return 1;
        runend_pos = get_prev_quot(runend_pos);
        runend = runend_value(runend_pos);
    }
    return 0;
}


uint64_t Cqf::remove(uint64_t number){
    //get quotient q and remainder r
    uint64_t quot = quotient(number);
    uint64_t rem = remainder(number);

    if (is_occupied(quot) == false) return 0;

    // TODO:
    // THIS SHOULD USE QUERY AND THEN REMOVE

    // GET POSITION

    // REMOVE ELEMENT BY SHIFTING EVERYTHING LEFT OF ONE REMINDER

    // MOVE 1 LEFT THE RUNEND OR CLEAR IT WITH OCCUPIED

    // ADJ OFFSET IF NEEDED

}


/*

MID LEVEL OPERATIONS

*/

// TO FINISH
uint64_t Cqf::shift_right_and_set(uint64_t Start_quotient,uint64_t end_quotient, uint64_t next_remainder){
    assert(Start_quotient < end_quotient);
    assert(end_quotient < ( 1ULL << quotient_size));

    uint64_t curr_word_pos = get_remainder_word_position(Start_quotient);
    uint64_t curr_word_shift = get_remainder_shift_position(Start_quotient);

    uint64_t end_word_pos = get_remainder_word_position(end_quotient);
    uint64_t end_word_shift = get_remainder_shift_position(end_quotient);

    assert(end_word_pos * MEM_UNIT + end_word_shift + remainder_size < num_bits());

    uint64_t to_shift = 0;
    //uint64_t word_difference = word_end_pos - word_start_pos;

    //case in which I am going to end shifting in a different word and
    // the remainder to set is between two different words
    // just the first operation in the word, then is the same 
    if ((curr_word_pos < end_word_pos) && (curr_word_shift + remainder_size >= MEM_UNIT)){

        to_shift = get_bits(curr_word_pos,curr_word_shift,remainder_size);

        set_bits(curr_word_pos,curr_word_shift,next_remainder, remainder_size);

        next_remainder = to_shift;
        curr_word_shift = remainder_size - (MEM_UNIT - curr_word_shift);
        curr_word_pos = get_next_remainder_word(curr_word_pos);
    }

    while (curr_word_pos < end_word_pos){

        to_shift = get_bits(curr_word_pos,curr_word_shift,MEM_UNIT-curr_word_shift);

        set_bits(curr_word_pos,curr_word_shift,next_remainder, remainder_size);

        set_bits(curr_word_pos,curr_word_shift + remainder_size,to_shift, MEM_UNIT - curr_word_shift - remainder_size);

        next_remainder = to_shift >> (MEM_UNIT - curr_word_shift - remainder_size);
        curr_word_shift = 0;
        curr_word_pos = get_next_remainder_word(curr_word_pos);
    }

    //save the bits that are gonna be moved.
    to_shift = get_bits(curr_word_pos,curr_word_shift,end_word_shift-curr_word_shift);

    //set the new_remainder bits into the word
    set_bits(curr_word_pos,curr_word_shift,next_remainder,remainder_size);

    //set the rest of the bits that had to be shifted
    set_bits(curr_word_pos,curr_word_shift+remainder_size,to_shift,end_word_shift-curr_word_shift);

    // HERE I SHOULD FIX THE RUNEND AND OCCUPIEDS AND OFFSET WORDS

}


uint64_t Cqf::get_remainder(uint64_t position) const{

    int64_t block = get_block(position);
    uint64_t pos_in_block = get_shift(position);

    uint64_t pos = (block*((MET_UNIT+remainder_size)*MEM_UNIT)+MET_UNIT*MEM_UNIT+pos_in_block*remainder_size);

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

// find first unused slot given a position
uint64_t Cqf::first_unused_slot(uint64_t quotient) const{

    uint64_t rend_pos = get_runend_pos(quotient);
    while (quotient <= rend_pos){
        quotient = get_next_quot(rend_pos);
        rend_pos = (quotient);
    }
    return quotient;
}

uint64_t Cqf::get_runend_pos(uint64_t quotient) const{

    uint64_t block = get_block(quotient);
    uint64_t pos_in_block = get_shift(quotient);
    uint64_t offset = get_offset(block);
    uint64_t occupied = get_occupieds(block);
    uint64_t runends = get_runends(block);

    uint64_t position = bitrank(occupied,pos_in_block);
    position += offset;
    uint64_t select = bitselect(runends,position);

    while (select == MEM_UNIT){

        block = get_next_block(block);
        position -= bitrank(runends,MEM_UNIT-1);
        uint64_t runends = get_runends(block);
        select = bitselect(runends,position);
        
    }

    return block * MEM_UNIT + select;
}

std::pair<uint64_t,uint64_t> Cqf::get_run_pos(uint64_t quotient) const{

    uint64_t block = get_block(quotient);
    uint64_t pos_in_block = get_shift(quotient);
    uint64_t offset = get_offset(block);
    uint64_t occupied = get_occupieds(block);
    uint64_t runends = get_runends(block);

    uint64_t position = bitrank(occupied,pos_in_block);
    position += offset;
    uint64_t select = bitselect(runends,position);

    std::pair<uint64_t, uint64_t> boundaries;

    while (select == MEM_UNIT){

        block = get_next_block(block);
        position -= bitrank(runends,MEM_UNIT-1);
        uint64_t runends = get_runends(block);
        select = bitselect(runends,position);
        
    }
    
    uint64_t start = block * MEM_UNIT + bitselect(runends,position-1);
    if (start > quotient) boundaries.first = start;
    else boundaries.first = quotient;

    boundaries.second = block * MEM_UNIT + select;

    return boundaries;
}


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


uint64_t Cqf::get_reminder_block(uint64_t slot) const{
    return 0;
}


uint64_t Cqf::set_reminder_block(uint64_t slot){
    return 0;
}


uint64_t Cqf::remove_reminder_block(uint64_t slot){
    return 0;
}


bool Cqf::is_occupied_block(uint64_t position) const{
    return 0;
}



/*

CIRCULAR CQF

*/
uint64_t Cqf::get_next_block(uint64_t current_block) const{
    if (current_block < number_blocks - 1) return ++current_block;
    else return 0; 
}

uint64_t Cqf::get_prev_block(uint64_t current_block) const{
    if (current_block > 0) return --current_block;
    else return number_blocks - 1; 
}

uint64_t Cqf::get_next_remainder_word(uint64_t current_word) const{
    uint64_t current_block = current_word / (MET_UNIT + remainder_size);
    uint64_t pos_in_block = current_word % (MET_UNIT + remainder_size);

    if (pos_in_block > (MET_UNIT-1)) return ++current_word;
    else{
        uint64_t next_block = get_next_block(current_block);
        return next_block * (MET_UNIT + remainder_size) + (MET_UNIT - 1); 
    }
}

uint64_t Cqf::get_prev_remainder_word(uint64_t current_word) const{
    uint64_t current_block = current_word / (MET_UNIT + remainder_size);
    uint64_t pos_in_block = current_word % (MET_UNIT + remainder_size);

    if (pos_in_block > MET_UNIT) return --current_word;
    else{
        uint64_t prev_block = get_next_block(current_block);
        return prev_block * (MET_UNIT + remainder_size) + (MET_UNIT + remainder_size - 1); 
    }
}

uint64_t Cqf::get_next_quot(uint64_t current_quot) const{
    if (current_quot < number_blocks*MEM_UNIT - 1) return ++current_quot;
    else return 0;
}

uint64_t Cqf::get_prev_quot(uint64_t current_quot) const{
    if (current_quot > 0) return --current_quot;
    else return number_blocks*MEM_UNIT - 1; 
}

/*

SMALL LEVEL OPERATIONS

*/
uint64_t Cqf::get_remainder_word_position(uint64_t quotient){
    return (get_block(quotient) * (MET_UNIT + remainder_size) + MET_UNIT + ((remainder_size * get_shift(quotient)) / MEM_UNIT)); 
}

uint64_t Cqf::get_remainder_shift_position(uint64_t quotient){
    return (get_shift(quotient) * remainder_size ) % MEM_UNIT;
}

void Cqf::set_word(uint64_t word, uint64_t pos){
    cqf[pos] = word;
}


void Cqf::set_bits(uint64_t block,uint64_t shift, uint64_t value, uint64_t len) {
    assert((block * MEM_UNIT + shift + len) <= num_bits());
    assert(len == MEM_UNIT or (value >> len) == 0);
    if (len == 0) return;

    uint64_t mask = mask_right(len);
    value &= mask; // to be sure

    cqf[block] &= ~(mask << shift);
    cqf[block] |= (value << shift);

    uint64_t stored = MEM_UNIT - shift;

    if (len > stored){
        cqf[block+1] &= ~(mask_right(len-stored));
        cqf[block+1] |= (value >> stored);
    }
}

void Cqf::set_bits(uint64_t pos, uint64_t value, uint64_t len) {
    assert(pos + len <= num_bits());
    assert(len == MEM_UNIT or (value >> len) == 0);
    if (len == 0) return;

    uint64_t mask = mask_right(len);
    uint64_t block = get_block(pos);
    uint64_t shift = get_shift(pos);
    value &= mask;

    cqf[block] &= ~(mask << shift);
    cqf[block] |= (value << shift);

    uint64_t stored = MEM_UNIT - shift;

    if (len > stored){
        cqf[block+1] &= ~(mask_right(len-stored));
        cqf[block+1] |= (value >> stored);
    }
}

uint64_t Cqf::get_bits(uint64_t word,uint64_t shift, uint64_t len) const {

    assert(word < cqf.size());

    if (!len) return 0;

    uint64_t mask = mask_right(len);

    if (shift + len <= MEM_UNIT) return (cqf[word] >> shift) & mask;

    return (cqf[word] >> shift) | ((cqf[word+1] << (MEM_UNIT - shift)) & mask);
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

std::vector<uint64_t> Cqf::get_slice(uint64_t pos, uint64_t len) const{
    uint64_t num_int = get_block(len);
    uint64_t rem = get_shift(len);
    std::vector<uint64_t> slice(num_int+1);

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
    std::cout << std::endl;
}


void Cqf::print_bits(uint64_t pos, uint64_t len) const {

    uint64_t bits = get_bits(pos,len);
    uint64_t b = bits;
    for (uint64_t j = 0; j < len; ++j){
            std::cout << (b & 0b1);
        b >>= 1;
    }
    std::cout << " | ";
    uint8_t* temp = (uint8_t*)&bits;
    for (uint64_t j = 0; j < (len/8)+1; ++j){
        uint8_t byte_num = *temp;
       std::cout << +byte_num << " ";
        temp++;
    } 
    std::cout << " | ";
    std::cout << bits;
    std::cout << std::endl;
}


void Cqf::print_word(uint64_t pos) const{
    assert(pos < number_blocks);
    uint64_t word = get_word(pos);
    std::cout << word << std::endl;
}

void Cqf::print_filter() const{
    for(uint64_t i = 0; i < number_blocks; ++i){
        uint64_t base = i*block_size;

        std::cout << "OFFSET" << std::endl;
        uint64_t word = get_word(base+OFF_POS);
        std::cout << word << std::endl;

        std::cout << "OCCUPIEDS" << std::endl;
        word = get_word(base+OCC_POS);
        std::cout << word << std::endl;

        std::cout << "RUNENDS" << std::endl;
        word = get_word(base+RUN_POS);
        std::cout << word << std::endl;

        for(uint64_t j = MET_UNIT; i < block_size; ++j){
            word = get_word(base+j);
            std::cout << word << std::endl;
        }
        
    }
    std::cout << std::endl;
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


uint64_t Cqf::get_num_bits() const{
    return m_num_bits;
}

uint64_t Cqf::get_quotient_size() const{
    return quotient_size;
}

uint64_t Cqf::get_remainder_size() const{
    return remainder_size;
}

uint64_t Cqf::get_number_blocks() const{
    return number_blocks;
}

uint64_t Cqf::get_block_size() const{
    return block_size;
}


/*
ADDITIONAL METHODS NOT USED BY THE CLASS
*/

uint64_t mask_left(uint64_t numbits){
    uint64_t mask = mask_right(numbits) << (MEM_UNIT - numbits);
    return mask;
}

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

