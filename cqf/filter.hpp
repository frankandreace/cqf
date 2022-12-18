
#ifndef FILTER_HPP
#define FILTER_HPP

#include <vector>

class Cqf {
    
    public:
    
    /*
    
    OPERATIONS ON THE ENTIRE CQF
    
    */
    // constructor
    Cqf(uint64_t quotient_s); 
    Cqf(uint64_t quotient_s, uint64_t n_blocks);

    //resize the cqf
    void resize(uint64_t new_size);

    // load the cqf from a file
    void load();

    // save the cqf into a file 
    void save();


    /*

    INFORMATION PROVIDING METHODS

    */
    //quotient of a given number
    uint64_t quotient(uint64_t number) const;

    //remainder of a given number
    uint64_t remainder(uint64_t number) const;

    // num bits occupied by the cqf
    uint64_t num_bits() const;

    // length of the uint64_t vector
    uint64_t num_64bit_words() const;

    // num ints inserted in the filter
    uint64_t num_reminders_in_filter() const;


    /*

    HIGH LEVEL OPERATIONS
    
    */

    // insert a new number in the filter
    uint64_t insert(uint64_t number);

    // query a number from the filter
    uint64_t query(uint64_t number) const;

    // remove(if present) a number from the filter
    uint64_t remove(uint64_t number);


    /*

    MID LEVEL OPERATIONS
    
    */

    //shift remainders on 1 block from START to END, inserting a new remainder in START.
    uint64_t shift_right_and_set(uint64_t Start_quotient,uint64_t end_quotient, uint64_t new_remainder);

    //shift remainders on 1 block from START to END, inserting a new remainder in START.
    uint64_t shift_right_and_set_circ(uint64_t Start_quotient,uint64_t end_quotient, uint64_t new_remainder);

    //shift left remainders on 1 block from START to END, clearing the END block.
    uint64_t shift_left_and_rem_circ(uint64_t start_quotient,uint64_t end_quotient);

    //update occupieds, runends and offsets based on the insertion
    void update_metadata_insertion(uint64_t quot_value,uint64_t start_pos,uint64_t end_pos);

    //update occupieds, runends and offsets based on the delition
    void update_metadata_deletion(std::pair<uint64_t,uint64_t> boundary, uint64_t start_pos, uint64_t end_pos, uint64_t quotient_pos);

    // find last element on the right that should be moved left when deleting a reminder from the filter
    uint64_t find_rightmost_to_shift_left(uint64_t start_pos, uint64_t end_slot) const;

    //get a reminder given a position
    uint64_t get_remainder(uint64_t position) const;

    //set a reminder on a specified position
    uint64_t set_remainder(uint64_t positon, uint64_t value);

    // remove a reminder given a number
    uint64_t remove_remainder(uint64_t reminder);

    //get the value of the runend at a certain position
    uint64_t runend_value(uint64_t position) const;
    
    // check if the occupied slot of a given position is set to 1
    bool is_occupied(uint64_t position) const;

    // find first unused slot given a position
    uint64_t first_unused_slot(uint64_t quotient) const;
    
    // give the result of select(rank(x))
    uint64_t sel_rank_filter(uint64_t quotient) const;

    // give the boundaries of a run of the filter
    std::pair<uint64_t,uint64_t> get_run_boundaries(uint64_t quotient) const;

    // get offset of the selected quotient
    uint64_t get_offset(uint64_t block, uint64_t pos_in_block) const;

    /*

    BLOCK LEVEL OPERATIONS
    
    */

    //perform the get_reminder at the block level
    //uint64_t get_reminder_block(uint64_t slot) const;

    // perform the set_reminder at the block level 
    //uint64_t set_reminder_block(uint64_t slot); 
    
    // perform the remove reminder at the block level
    //uint64_t remove_reminder_block(uint64_t slot); 

    // perform the is_occupied at the block level
    //bool is_occupied_block(uint64_t position) const; 


    /*

    CIRCULAR CQF

    */
    //get the next word where remainders are stored. used for shifting remainders. 
    uint64_t get_next_remainder_word(uint64_t current_word) const;
    //get the previous word where remainders are stored. used for shifting remainders. 
    uint64_t get_prev_remainder_word(uint64_t current_word) const;
    
    //get the next word where remainders are stored. used for shifting remainders. 
    uint64_t get_next_runend_word(uint64_t current_word) const;
    //get the previous word where remainders are stored. used for shifting remainders. 
    uint64_t get_prev_runend_word(uint64_t current_word) const;
    
    //get the next block number.  
    uint64_t get_next_block(uint64_t current_block) const;
    //get the prev block number. 
    uint64_t get_prev_block(uint64_t current_block) const;
    
    //get the next quotient
    uint64_t get_next_quot(uint64_t current_quot) const;
    //get the previous quotient
    uint64_t get_prev_quot(uint64_t current_quot) const;
    
    //get previous word in filter
    uint64_t get_prev_word(uint64_t current_w) const;
    //get next word in filter
    uint64_t get_next_word(uint64_t current_w) const;


    // get and set the occupieds word in the block number 'pos' of the cqf
    uint64_t get_occupieds_word(uint64_t pos) const; 
    void set_occupieds_bit(uint64_t position, uint64_t shift, uint64_t bit_value);
    void set_occupieds_word(uint64_t position, uint64_t word);
    // get and set the offset word in the block number 'pos' of the cqf
    uint64_t get_offset(uint64_t pos) const;
    void set_offset(uint64_t pos, uint64_t value);
    
    // get and set the runends word in the block number 'pos' of the cqf
    uint64_t get_runends(uint64_t pos) const;
    void set_runends(uint64_t pos, uint64_t value);  


    /*

    SMALL LEVEL OPERATIONS

    */
    uint64_t get_remainder_word_position(uint64_t quotient);

    uint64_t get_remainder_shift_position(uint64_t quotient);

    // rank operation 
    uint64_t rank(uint64_t value); 

    // select operation
    uint64_t select(uint64_t value); 

    // set bits in the CQF given the position, bits into a uint64_t and the number of bits to set
    void set_bits_remainders(uint64_t pos,uint64_t shift, uint64_t value, uint64_t len);
    void set_bits_runends(uint64_t pos,uint64_t shift, uint64_t value, uint64_t len);
    void set_bits(uint64_t block,uint64_t shift, uint64_t value, uint64_t len);

    // get bits from the CQF given a position and the number of bits to get
    uint64_t get_bits_remainders(uint64_t word_pos,uint64_t shift, uint64_t len) const;
    uint64_t get_bits_runends(uint64_t word_pos,uint64_t shift, uint64_t len) const;
    uint64_t get_bits(uint64_t block,uint64_t shift, uint64_t len) const ;

    // set an entire word in the CQF (64 bits) given a position
    void set_word(uint64_t word, uint64_t pos);

    // get an entire word in the CQF (64 bits) given a position
    uint64_t get_word(uint64_t pos) const;

    uint64_t get_bit_from_word(uint64_t word, uint64_t pos_bit) const;

    uint64_t shift_bits_left(uint64_t word_id,uint64_t shift, uint64_t insert, uint64_t len, uint64_t end);

    uint64_t shift_bits_right(uint64_t word_id,uint64_t shift, uint64_t insert, uint64_t len);

    /*

    PRINTING + DEBUGGING STUFF (SOME OF THEM SHOULD BE MOVED TO PRIVATE OR ELIMINATED)

    */

    std::vector<uint64_t> get_slice(uint64_t pos, uint64_t len) const;

    void print_slice(uint64_t pos, uint64_t len) const;
    void print_bits(uint64_t pos, uint64_t len) const;
    void print_word(uint64_t pos) const;
    void print_filter() const;

    uint64_t get_bits_custom(uint64_t pos, uint64_t len) const;
    void set_bits_custom(uint64_t pos, uint64_t value, uint64_t len); 

    uint64_t get_num_bits() const;
    uint64_t get_quotient_size() const;
    uint64_t get_remainder_size() const;
    uint64_t get_number_blocks() const;
    uint64_t get_block_size() const;

    private:
    // VALUES

    std::vector<uint64_t> cqf; // uint64_t vector to store the cqf
    uint64_t m_num_bits;    // max number of bits occupied by the cqf to check no memory leaks
    uint64_t quotient_size; // value of q
    uint64_t remainder_size;    // value of r
    uint64_t number_blocks; // number of blocks the cqf is divided into
    uint64_t block_size;

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

//give the quotient from a block and shift
uint64_t get_quot_from_block_shift(uint64_t block, uint64_t shift);


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