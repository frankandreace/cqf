
#ifndef FILTER_HPP
#define FILTER_HPP

#include <vector>

class Cqf {
    
    public:
    
    /*
    
    OPERATIONS ON THE ENTIRE CQF
    
    */
    // constructor
    Cqf( uint64_t num_blocks); 

    //resize the cqf
    void Cqf::resize(uint64_t new_size);

    // load the cqf from a file
    void Cqf::load();

    // save the cqf into a file 
    void Cqf::save();


    /*

    INFORMATION PROVIDING METHODS

    */
    //quotient of a given number
    uint64_t Cqf::quotient(uint64_t number) const;

    //remainder of a given number
    uint64_t Cqf::remainder(uint64_t number) const;

    // num bits occupied by the cqf
    uint64_t Cqf::num_bits() const;

    // length of the uint64_t vector
    uint64_t Cqf::num_64bit_words() const;

    // num ints inserted in the filter
    uint64_t Cqf::num_reminders_in_filter() const;


    /*

    HIGH LEVEL OPERATIONS
    
    */

    // insert a new number in the filter
    uint64_t Cqf::insert(uint64_t number);

    // query a number from the filter
    uint64_t Cqf::query(uint64_t number) const;

    // remove(if present) a number from the filter
    uint64_t Cqf::remove(uint64_t number);


    /*

    MID LEVEL OPERATIONS
    
    */

    //get a reminder given a position
    uint64_t Cqf::get_remainder(uint64_t position) const;

    //set a reminder on a specified position
    uint64_t Cqf::set_remainder(uint64_t positon, uint64_t value);

    // remove a reminder given a number
    uint64_t Cqf::remove_remainder(uint64_t reminder);

    //get the value of the runend at a certain position
    uint64_t Cqf::runend_value(uint64_t position) const;
    
    // check if the occupied slot of a given position is set to 1
    bool Cqf::is_occupied(uint64_t position) const;

    // find first unused slot given a position
    uint64_t Cqf::first_unused_slot(uint64_t position) const;
    
    // give the result of select(rank(x))
    uint64_t Cqf::runend_pos(uint64_t position) const;

    // get offset of the selected quotient
    uint64_t Cqf::get_offset(uint64_t block, uint64_t pos_in_block) const;

    /*
    Oi = SELECT(runends,RANK(occupieds,i)) - i, if > 0 else 0. 
    */
    void Cqf::set_offset(uint64_t value, uint64_t position);

    /*

    BLOCK LEVEL OPERATIONS
    
    */

    //perform the get_reminder at the block level
    uint64_t Cqf::get_reminder_block(uint64_t slot) const;

    // perform the set_reminder at the block level 
    uint64_t Cqf::set_reminder_block(uint64_t slot); 
    
    // perform the remove reminder at the block level
    uint64_t Cqf::remove_reminder_block(uint64_t slot); 

    // perform the is_occupied at the block level
    bool Cqf::is_occupied_block(uint64_t position) const; 

    // perform the runend_position at the block level
    uint64_t Cqf::runend_pos_block(uint64_t position) const;


    /*

    SMALL LEVEL OPERATIONS

    */

    // rank operation 
    uint64_t Cqf::rank(uint64_t value); 

    // select operation
    uint64_t Cqf::select(uint64_t value); 

    // set bits in the CQF given the position, bits into a uint64_t and the number of bits to set
    void Cqf::set_bits(uint64_t pos, uint64_t set_bits, uint64_t len); 

    // get bits from the CQF given a position and the number of bits to get
    uint64_t Cqf::get_bits(uint64_t pos, uint64_t len) const;

    // set an entire word in the CQF (64 bits) given a position
    void Cqf::set_word(uint64_t word, uint64_t pos);

    // get an entire word in the CQF (64 bits) given a position
    uint64_t Cqf::get_word(uint64_t pos) const;

    uint64_t Cqf::get_bit_from_word(uint64_t word, uint64_t pos_bit) const;


    /*

    PRINTING + DEBUGGING STUFF (SOME OF THEM SHOULD BE MOVED TO PRIVATE OR ELIMINATED)

    */

    std::vector<uint64_t> get_slice(uint64_t pos, uint64_t len) const;

    void print_slice(uint64_t pos, uint64_t len) const;
    void print_bits(uint64_t pos, uint64_t len) const;
    void print_word(uint64_t pos) const;

    // get the offset bitvector number 'pos' in the cqf
    uint64_t get_offset(uint64_t pos) const; 

    // get the occupieds bitvector number 'pos' in the cqf
    uint64_t get_occupieds(uint64_t pos) const; 
    
    // get the runends bitvector number 'pos' in the cqf
    uint64_t get_runends(uint64_t pos) const;  

    bool contains(uint64_t number) const;



    private:

    // VALUES

    std::vector<uint64_t> cqf; // uint64_t vector to store the cqf
    uint64_t m_num_bits;    // max number of bits occupied by the cqf to check no memory leaks
    uint64_t quotient_size; // value of q
    uint64_t remainder_size;    // value of r
    uint64_t number_blocks; // number of blocks the cqf is divided into

    // FUNCTIONS

};



/*

ADDITIONAL METHODS

*/

// get a mask with numbits 1s from the right
uint64_t mask_right(uint64_t numbits); 

// get the block number given a quotient
uint64_t get_block(uint64_t position); 

//give the position of the quotient in the block
uint64_t get_shift(uint64_t position); 



/*

    ROB PATRO'S PAPER IMPLEMENTATIONS

*/

// Rob Patro's implementation operations
uint64_t bitselect(uint64_t num, uint64_t rank); 

// Rob Patro's implementation operations
uint64_t popcnt(uint64_t num); 

// Rob Patro's implementation operations
uint64_t bitrank(uint64_t val, uint64_t pos); 


#endif