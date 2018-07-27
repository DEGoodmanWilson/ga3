//
// Created by Don Goodman-Wilson on 05/07/2018.
//

#include <catch.hpp>
#include <ga3/chromosome.hpp>

// TODO this is fucking ugly. There has to be a better way
static constexpr uint64_t min{1};
static constexpr uint64_t max{10};
static constexpr uint64_t size{10};
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

SCENARIO("chromosomes")
{
    GIVEN("a newly constructed chromosome")
    {
        THEN("it should be initialized with N random numbers in the appropriate range")
        {
            ga3::chromosome<size> chromo;
            for (int i = 0; i < size; ++i)
            {
                REQUIRE(chromo[i] >= min);
                REQUIRE(chromo[i] <= max);
            }
        }
        THEN("We ought to be able to set particular genes")
        {
            ga3::chromosome<size> chromo;
            constexpr uint64_t value{2};
            for (int i = 0; i < size; ++i)
            {
                chromo[i] = value;
            }
            for (int i = 0; i < size; ++i)
            {
                REQUIRE(chromo[i] == value);
            }
        }
    }
    GIVEN("two genes")
    {
        THEN("we should be able to splice them with 1-point crossover")
        {
            ga3::chromosome<size> chromo1, chromo2;
            constexpr uint64_t value1{1}, value2{2};

            for (int i = 0; i < size; ++i)
            {
                chromo1[i] = value1;
                chromo2[i] = value2;
            }

            ga3::chromosome<size>::set_crossover(ga3::crossover_kind_t::one_point);

            auto new_chromo = chromo1 + chromo2;

            // the result is stochastic. How to test?
            REQUIRE(new_chromo[0] == value1);
            REQUIRE(new_chromo[9] == value2);
        }

        THEN("we should be able to splice them with 2-point crossover")
        {
            ga3::chromosome<size> chromo1, chromo2;
            constexpr uint64_t value1{1}, value2{2};

            for (int i = 0; i < size; ++i)
            {
                chromo1[i] = value1;
                chromo2[i] = value2;
            }

            ga3::chromosome<size>::set_crossover(ga3::crossover_kind_t::two_point);

            auto new_chromo = chromo1 + chromo2;

            // the result is stochastic. How to test?
            REQUIRE(new_chromo[0] == value1);
            REQUIRE(new_chromo[5] == value2);
            REQUIRE(new_chromo[9] == value1);
        }

        THEN("we should be able to splice them with uniform crossover")
        {
            ga3::chromosome<size> chromo1, chromo2;
            constexpr uint64_t value1{0}, value2{1};

            for (int i = 0; i < size; ++i)
            {
                chromo1[i] = value1;
                chromo2[i] = value2;
            }

            ga3::chromosome<size>::set_crossover(ga3::crossover_kind_t::uniform);

            // this is stochastic. So, let's add chromos a ton of times, and look at the distributions for each gene
            std::array<uint64_t, size> sum{0,0,0,0,0,0,0,0,0,0};
            for(int i = 0; i < 10000; ++i)
            {
                auto new_chromo = chromo1 + chromo2;
                for(int i = 0; i < size; ++i)
                {
                    sum[i] += new_chromo[i];
                }
            }

            // the result is stochastic. How to test?
            for(int i = 0; i < size; ++i)
            {
                REQUIRE(sum[i] >= 4800);
                REQUIRE(sum[i] <= 5200);
            }
        }
    }
    GIVEN("A non-uniform set of bounds")
    {
        THEN("they should be respected during chromosome initialization")
        {
            ga3::chromosome<4> chromo;

            for(int i = 0; i < 4; ++i)
            {
                REQUIRE(chromo[i] >= i*2);
                REQUIRE(chromo[i] <= (i*2)+1);
            }
        }
    }
}