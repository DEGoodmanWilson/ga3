//
// Created by Don Goodman-Wilson on 05/07/2018.
//

#include <catch.hpp>
#include <ga3/chromosome.hpp>

// TODO this is fucking ugly.
template<>
std::array<ga3::gene_range, 2ul> ga3::chromosome<2ul>::gene_bounds =
        {
                {{1, 2}, {1, 2}}
        };

SCENARIO("chromosomes")
{
    GIVEN("a newly constructed chromosome")
    {
        THEN("it should be initialized with N random numbers")
        {
            const int size{2};
            ga3::chromosome<size> chromo;
            auto genes = chromo.get_genes();
            REQUIRE(size == genes.size());
            for (int i = 0; i < size; ++i)
            {
                REQUIRE(genes[i] >= 0);
                REQUIRE(genes[i] <= UINT64_MAX);
            }
        }
    }
}