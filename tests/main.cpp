//
// Created by Don Goodman-Wilson on 05/07/2018.
//

#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include "test.h"

// TODO this is fucking ugly. There has to be a better way
template<>
std::array<ga3::gene_range, size> ga3::chromosome<size>::gene_bounds =
        {{
                 {min, max},
                 {min, max},
                 {min, max},
                 {min, max},
                 {min, max},
                 {min, max},
                 {min, max},
                 {min, max},
                 {min, max},
                 {min, max},
         }};

template<>
std::array<ga3::gene_range, 4> ga3::chromosome<4>::gene_bounds =
        {{
                 {0, 1},
                 {2, 3},
                 {4, 5},
                 {6, 7},
         }};


SCENARIO("sanity check")
{
    GIVEN("when the tests run")
    {
        THEN("they should actually run")
        {
            REQUIRE(true);
        }
    }
}