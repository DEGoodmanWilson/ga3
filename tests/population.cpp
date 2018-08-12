//
// Created by Don Goodman-Wilson on 27/07/2018.
//

#include <catch.hpp>
#include "test.h"
#include <algorithm>
#include <atomic>

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

static ga3::chromosome::evaluation_function_t default_fitness_function
        {
                [](std::vector<ga3::gene> &genes) -> double
                {
                    return 0;
                }
        };

static std::atomic_uint8_t threads{0}, peak_threads{0};
static ga3::chromosome::evaluation_function_t counter
        {
                [](std::vector<ga3::gene> &genes) -> double
                {
                    threads++;
                    if (threads > peak_threads)
                    {
                        uint8_t temp = threads;
                        peak_threads = temp;
                    }
                    threads--;
                    return std::accumulate(genes.begin(), genes.end(), 0);
                }
        };

SCENARIO("populations")
{
    GIVEN("an initial population")
    {
        THEN("it should initialize correctly")
        {
            ga3::population pop{pop_size, gene_bounds_10, default_fitness_function};

            // how do we even verify this. We just count the number of 0s and 1s in the population I guess.
            std::array<uint64_t, size_ten> sum{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (uint64_t i = 0; i < pop_size; ++i)
            {
                for (uint64_t j = 0; j < size_ten; ++j)
                {
                    sum[j] += pop.at(i).at(j);
                }
            }

            for (uint64_t j = 0; j < size_ten; ++j)
            {
                REQUIRE(sum[j] >= 4800);
                REQUIRE(sum[j] <= 5200);
            }
        }

        THEN("it should be able to tell us the most fit chromosome without threading")
        {
            threads = peak_threads = 0;
            ga3::population pop{pop_size, gene_bounds_10, counter, 0};
            const auto start_fitness = pop.evaluate().evaluate();

            REQUIRE(start_fitness == 10);
            REQUIRE(peak_threads == 1);
        }

        THEN("it should be able to tell us the most fit chromosome with threading")
        {
            threads = 0;
            peak_threads = 0;

            ga3::population pop{pop_size, gene_bounds_10, counter};
            const auto start_fitness = pop.evaluate().evaluate();

            REQUIRE(start_fitness == 10);
            REQUIRE(peak_threads > 1);
        }

        THEN("it should be able to converge on a solution using roulette selection and generational replacement")
        {
            ga3::population::set_selection(ga3::population::selection_kind_t::roulette);
            ga3::population::set_replacement(ga3::population::replacement_kind_t::generational);
            ga3::population pop{5, gene_bounds_10, counter};
            const auto start_fitness = pop.evaluate().evaluate();

            pop.evolve(10); //10 generations
            const auto end_fitness = pop.evaluate().evaluate();

            REQUIRE(end_fitness < start_fitness);
            // TODO how to test that generational replacement is working?
        }
    }
}