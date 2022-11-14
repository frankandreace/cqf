# cqf
Counting Quotient Filters

# Compiling
`cmake -S src -B build`
`cd build && make`

# Executing
In `build` folder:
`./CQF <example.dat>` where `example.dat` is a file containing `uint64_t`'s written as bytes with no separator.

rm -rf build/ && cmake -S . -B build && cd build && make && clear

./cqf/CQF > ../rank_select.txt && cd ..