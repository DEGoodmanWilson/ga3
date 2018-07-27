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

SCENARIO("chromosomes")
{
    GIVEN("a newly constructed chromosome")
    {
        THEN("it should be initialized with N random numbers in the appropriate range")
        {
            ga3::chromosome<size> chromo;
            auto genes = chromo.get_genes();
            REQUIRE(size == genes.size());
            for (int i = 0; i < size; ++i)
            {
                REQUIRE(genes[i] >= min);
                REQUIRE(genes[i] <= max);
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
            auto genes = chromo.get_genes();
            for (int i = 0; i < size; ++i)
            {
                REQUIRE(genes[i] == value);
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
    }
}