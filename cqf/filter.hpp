
#ifndef FILTER_HPP
#define FILTER_HPP

#include <vector>

class Cqf {
    public:
    
    Cqf( uint64_t num_blocks); // constructor
    uint64_t num_bits() const;
    uint64_t num_64bit_words() const;
    void set_bits(uint64_t pos, uint64_t set_bits, uint64_t len);
    std::vector<uint64_t> get_slice(uint64_t pos, uint64_t len) const;
    uint64_t get_bits(uint64_t pos, uint64_t len) const;
    void print_slice(uint64_t pos, uint64_t len) const;
    void print_bits(uint64_t pos, uint64_t len) const;

    private:
    std::vector<uint64_t> cqf;
    uint64_t m_num_bits;
};

#endif