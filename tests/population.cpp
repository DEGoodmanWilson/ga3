//
// Created by Don Goodman-Wilson on 27/07/2018.
//

#include <catch.hpp>
#include "test.h"
#include <algorithm>
#include <atomic>
#include <thread>

static const uint64_t min_0{0};
static const uint64_t max_1{1};
static const uint64_t max_10{10};

static constexpr uint64_t size_ten{10};
static constexpr uint64_t pop_size{30};
static const std::vector<ga3::gene_range> gene_bounds_10 =
        {{
                 {min_0, max_1},
                 {min_0, max_1},
                 {min_0, max_1},
                 {min_0, max_1},
                 {min_0, max_1},
                 {min_0, max_1},
                 {min_0, max_1},
                 {min_0, max_1},
                 {min_0, max_1},
                 {min_0, max_1},
         }};

static const std::vector<ga3::gene_range> gene_bounds_10_wide =
        {{
                 {min_0, max_10},
                 {min_0, max_10},
                 {min_0, max_10},
                 {min_0, max_10},
                 {min_0, max_10},
                 {min_0, max_10},
                 {min_0, max_10},
                 {min_0, max_10},
                 {min_0, max_10},
                 {min_0, max_10},
         }};

static ga3::chromosome::evaluation_function_t default_fitness_function
        {
                [](std::vector<ga3::gene> &genes) -> double
                {
                    return 0;
                }
        };

static std::atomic_uint16_t threads{0}, peak_threads{0};
static std::atomic_uint64_t count{0};
static ga3::chromosome::evaluation_function_t counter_sleep
        {
                [](std::vector<ga3::gene> &genes) -> double
                {
                    count++;
                    threads++;
                    if (threads > peak_threads)
                    {
                        uint8_t temp = threads;
                        peak_threads = temp;
                    }
                    double retval = std::accumulate(genes.begin(), genes.end(), 0);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));

                    threads--;


                    return retval;
                }
        };
static ga3::chromosome::evaluation_function_t counter
        {
                [](std::vector<ga3::gene> &genes) -> double
                {
                    count++;
                    threads++;
                    if (threads > peak_threads)
                    {
                        uint8_t temp = threads;
                        peak_threads = temp;
                    }
                    double retval = std::accumulate(genes.begin(), genes.end(), 0);

                    threads--;


                    return retval;
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
            uint64_t sum{0};
            for (uint64_t i = 0; i < pop_size; ++i)
            {
                for (uint64_t j = 0; j < size_ten; ++j)
                {
                    sum += pop.at(i).at(j);
                }
            }

            const auto expected_fitness = pop_size * gene_bounds_10.size() * 0.5; // we expect half to be 1's and half 0's
            const auto lower_expected_fitness = expected_fitness - (expected_fitness * 0.1);
            const auto upper_expected_fitness = expected_fitness + (expected_fitness * 0.1);
            REQUIRE(sum >= lower_expected_fitness);
            REQUIRE(sum <= upper_expected_fitness);
        }

        THEN("it should be able to tell us the most fit chromosome with threading")
        {
            threads = 0;
            peak_threads = 0;
            count = 0;

            ga3::population::single_threaded = false;
            ga3::population pop{pop_size, gene_bounds_10, counter_sleep};
            pop.evaluate().evaluate();

            REQUIRE(peak_threads >= 7);
            REQUIRE(count == pop_size);
        }

        THEN("it should be able to tell us the most fit chromosome without threading")
        {
            threads = 0;
            peak_threads = 0;
            count = 0;

            ga3::population::single_threaded = true;
            ga3::population pop{pop_size, gene_bounds_10, counter};
            pop.evaluate().evaluate();

            REQUIRE(peak_threads == 1);
            REQUIRE(count == pop_size);

            ga3::population::single_threaded = false;
        }

        THEN("it should be able to converge on a solution using roulette selection and generational replacement")
        {
            ga3::population pop{100, gene_bounds_10_wide, counter, ga3::population::selection_kind_t::roulette,
                                ga3::population::replacement_kind_t::generational};

            const auto start_fitness = pop.evaluate().evaluate();

            pop.evolve(100); //100 generations
            const auto end_fitness = pop.evaluate().evaluate();

            REQUIRE(end_fitness > start_fitness);
            // TODO how to test that generational replacement is working?
        }

        THEN("it should be able to converge on a solution using roulette selection and generational replacement and uniform crossover")
        {
            ga3::population pop{100, gene_bounds_10_wide, counter, ga3::population::selection_kind_t::roulette,
                                ga3::population::replacement_kind_t::generational,
                                ga3::chromosome::crossover_kind_t::uniform};

            const auto start_fitness = pop.evaluate().evaluate();

            pop.evolve(100); //100 generations
            const auto end_fitness = pop.evaluate().evaluate();

            REQUIRE(end_fitness > start_fitness);
            // TODO how to test that generational replacement is working?
        }

        THEN("it should be able to converge on a solution using roulette selection and steady-state replacement")
        {
            ga3::population pop{100, gene_bounds_10_wide, counter, ga3::population::selection_kind_t::roulette,
                                ga3::population::replacement_kind_t::steady_state};

            const auto start_fitness = pop.evaluate().evaluate();

            pop.evolve(100); //10 generations
            const auto end_fitness = pop.evaluate().evaluate();

            REQUIRE(end_fitness > start_fitness);
            // TODO how to test that generational replacement is working?
        }

        THEN("it should be able to converge on a solution using ranked selection and generational replacement")
        {
            ga3::population pop{100, gene_bounds_10_wide, counter, ga3::population::selection_kind_t::ranked,
                                ga3::population::replacement_kind_t::generational};

            const auto start_fitness = pop.evaluate().evaluate();

            pop.evolve(100); //10 generations
            const auto end_fitness = pop.evaluate().evaluate();

            REQUIRE(end_fitness > start_fitness);
            // TODO how to test that generational replacement is working?
        }

        THEN("it should be able to converge on a solution using ranked selection and steady-state replacement")
        {
            ga3::population::single_threaded = true;

            ga3::population pop{100, gene_bounds_10_wide, counter, ga3::population::selection_kind_t::ranked,
                                ga3::population::replacement_kind_t::steady_state};

            const auto start_fitness = pop.evaluate().evaluate();

            pop.evolve(100); //10 generations
            const auto end_fitness = pop.evaluate().evaluate();

            REQUIRE(end_fitness > start_fitness);
            // TODO how to test that generational replacement is working?

            ga3::population::single_threaded = false;
        }

        THEN("it should execute a post-generation hook")
        {
            ga3::population pop{10, gene_bounds_10_wide, default_fitness_function};

            uint64_t i{0};
            pop.evolve(100, [&]()
            {
                ++i;
            });

            REQUIRE(i == 100);
            // TODO how to test that generational replacement is working?
        }
    }
}