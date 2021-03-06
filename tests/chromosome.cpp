//
// Created by Don Goodman-Wilson on 05/07/2018.
//

#include <catch.hpp>
#include "test.h"

static const uint64_t min{1};
static const uint64_t max{3};
static constexpr uint64_t size{10};
static const auto one_point{ga3::chromosome::crossover_kind_t::one_point};
static const auto two_point{ga3::chromosome::crossover_kind_t::two_point};
static const auto uniform{ga3::chromosome::crossover_kind_t::uniform};

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

static const std::vector<ga3::gene_range> gene_bounds_4 =
        {{
                 {0, 1},
                 {2, 3},
                 {4, 5},
                 {6, 7},
         }};

static ga3::chromosome::evaluation_function_t default_fitness_function
        {
                [](std::vector<ga3::gene> &genes) -> double
                {
                    return 0;
                }
        };

static uint8_t call_count{0};

static ga3::chromosome::evaluation_function_t evaluation_function = [](std::vector<ga3::gene> &genes) -> double
{
    call_count++;
    return 1.0;
};

SCENARIO("chromosomes")
{
    GIVEN("a newly constructed chromosome")
    {
        ga3::chromosome chromo{gene_bounds_10, one_point, default_fitness_function};

        THEN("it should be initialized with N random numbers in the appropriate range")
        {
            for (int i = 0; i < size; ++i)
            {
                REQUIRE(chromo[i] >= min);
                REQUIRE(chromo[i] <= max);
            }
        }
        THEN("We ought to be able to set particular genes")
        {
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
        THEN("We should be able to examine the genes")
        {
            bool identical{false};
            auto my_genes = chromo.get_genes();
            REQUIRE(my_genes.size() == gene_bounds_10.size());
            for (uint8_t i = 0; i < my_genes.size(); ++i)
            {
                REQUIRE(my_genes.at(i) == chromo.at(i));
            }
        }
        THEN("Mutation should work!")
        {
            auto old_chromo{chromo};
            chromo.mutate();
            bool changed{false};
            for (uint8_t i = 0; i < gene_bounds_10.size(); ++i)
            {
                if (old_chromo.at(i) != chromo.at(i))
                {
                    changed = true;
                    break;
                }
            }
            REQUIRE(changed == true);
        }
    }
    GIVEN("a chromosome whose fitness is negative")
    {
        THEN("evaluate() should throw")
        {
            ga3::chromosome chromo{gene_bounds_10, one_point, [](std::vector<ga3::gene> &genes) -> double
            {
                return -1.0;
            }};

            REQUIRE_THROWS(chromo.evaluate());
        }
    }
    GIVEN("a chromosome that has not been evaluated")
    {
        THEN("get_fitness() should throw")
        {
            ga3::chromosome chromo{gene_bounds_10, one_point, default_fitness_function};

            REQUIRE_THROWS(chromo.get_fitness());
        }
    }
    GIVEN("two genes")
    {
        THEN("we should be able to splice them with 1-point crossover")
        {
            ga3::chromosome chromo1{gene_bounds_10, one_point, default_fitness_function},
                    chromo2{gene_bounds_10, one_point, default_fitness_function};
            constexpr uint64_t value1{1}, value2{2};

            for (int i = 0; i < size; ++i)
            {
                chromo1[i] = value1;
                chromo2[i] = value2;
            }

            auto new_chromo = chromo1 + chromo2;

            // the result is stochastic. How to test?
            REQUIRE(new_chromo[0] == value1);
            REQUIRE(new_chromo[9] == value2);
        }

        THEN("we should be able to splice them with 2-point crossover")
        {
            ga3::chromosome chromo1{gene_bounds_10, two_point, default_fitness_function},
                    chromo2{gene_bounds_10, one_point, default_fitness_function};
            constexpr uint64_t value1{1}, value2{2};

            for (int i = 0; i < size; ++i)
            {
                chromo1[i] = value1;
                chromo2[i] = value2;
            }

            auto new_chromo = chromo1 + chromo2;

            // the result is stochastic. How to test?
            // we can look for the crossover points.
            int64_t co_point_1{-1}, co_point_2{-1};
            for (int i = 0; i < size; ++i)// TODO allow iteration by iterator
            {
                if ((co_point_1 == -1) && (new_chromo[i] == value2))
                {
                    co_point_1 = i;
                }
                else if ((co_point_1 > -1) && (new_chromo[i] == value1))
                {
                    co_point_2 = i;
                }
            }
            REQUIRE(co_point_1 > -1);
            REQUIRE(co_point_2 > co_point_1);
        }

        THEN("we should be able to splice them with uniform crossover")
        {
            ga3::chromosome chromo1{gene_bounds_10, uniform, default_fitness_function},
                    chromo2{gene_bounds_10, one_point, default_fitness_function};
            constexpr uint64_t value1{0}, value2{1};

            for (int i = 0; i < size; ++i)
            {
                chromo1[i] = value1;
                chromo2[i] = value2;
            }

            // this is stochastic. So, let's add chromos a ton of times, and look at the distributions for each gene
            std::array<uint64_t, size> sum{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (int i = 0; i < 10000; ++i)
            {
                auto new_chromo = chromo1 + chromo2;
                for (int i = 0; i < size; ++i)
                {
                    sum[i] += new_chromo[i];
                }
            }

            // the result is stochastic. How to test?
            for (int i = 0; i < size; ++i)
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
            ga3::chromosome chromo{gene_bounds_4, one_point, default_fitness_function};

            for (int i = 0; i < 4; ++i)
            {
                REQUIRE(chromo[i] >= i * 2);
                REQUIRE(chromo[i] <= (i * 2) + 1);
            }
        }
    }

    GIVEN("A trivial fitness function")
    {
        THEN("then the fitness should be calculated exactly once.")
        {
            ga3::chromosome chromo{gene_bounds_4, one_point, evaluation_function};
            call_count = 0;

            auto fitness = chromo.evaluate();
            fitness = chromo.evaluate();
            fitness = chromo.evaluate();

            REQUIRE(fitness == 1.0);
            REQUIRE(call_count == 1);
        }
    }
}