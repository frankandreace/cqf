#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>     // for std::chrono::high_resolution_clock
#include <string.h>

using namespace std;

#define MEM_UNIT 64

class Cqf {

    private:
    uint64_t * cqf;

    public:
    Cqf( uint64_t number_of_blocks ); // constructor
    ~Cqf();                           // destructor 



    Cqf( uint64_t number_of_blocks ){ // constructor

        cqf = new uint64_t[(MEM_UNIT * number_of_blocks)]; 
        memset(cqf, 0, MEM_UNIT * number_of_blocks);
    }

    ~Cqf(){                            // destructor 
        free(cqf);
    }
    
};
