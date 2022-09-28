
add_library(cqf
filter.cpp
)

add_executable(CQF
tests.cpp
filter.cpp
main.cpp
)