
#ifndef FILTER_HPP
#define FILTER_HPP

class Cqf {
    public:
    
    Cqf( uint64_t number_of_blocks ); // constructor
    ~Cqf();                           // destructor 

    void printVector(uint64_t start, uint64_t end);
    void setInt(uint64_t start, uint64_t number);
    void setBits(uint64_t start,uint64_t offset, uint64_t length);
    void printBits(uint64_t startbit, uint64_t length);

    private:
    uint64_t * cqf;
    uint64_t len_cqf;
};

#endif