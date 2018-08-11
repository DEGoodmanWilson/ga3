//
// Created by Don Goodman-Wilson on 27/07/2018.
//

#include <catch.hpp>
#include "test.h"

static const uint64_t min{0};
static const uint64_t max{1};
static constexpr uint64_t size_ten{10};
static constexpr uint64_t pop_size{10000};
static const std::vector<ga3::gene_range> gene_bounds_10 =
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

SCENARIO("populations")
{
    GIVEN("an initial population")
    {
        THEN("it should initialize correctly")
        {
            ga3::population pop{pop_size, gene_bounds_10};

            // how do we even verify this. We just count the number of 0s and 1s in the population I guess.
            std::array<uint64_t, size_ten> sum{0,0,0,0,0,0,0,0,0,0};
            for(uint64_t i = 0; i < pop_size; ++i)
            {
                for(uint64_t j = 0; j < size_ten; ++j)
                {
                    sum[j] += pop.at(i).at(j);
                }
            }

            for(uint64_t j = 0; j < size_ten; ++j)
            {
                REQUIRE(sum[j] >= 4800);
                REQUIRE(sum[j] <= 5200);
            }
        }
    }
}