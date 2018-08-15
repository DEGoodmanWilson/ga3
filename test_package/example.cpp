#include <iostream>
#include <ga3/ga3.hpp>

int main() {
    ga3::population pop{
            1,
            {{0,1}},
            [](auto genes) -> auto
            {
                return 1.0;
            }
    };
    std::cout << "GA3 package testing: OK!" << std::endl;
}

