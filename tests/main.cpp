//
// Created by Don Goodman-Wilson on 05/07/2018.
//

#define CATCH_CONFIG_MAIN

#include <catch.hpp>

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