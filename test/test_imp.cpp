#include <catch2/catch_test_macros.hpp>
#include <imp/read_file.hpp>
#include <noam/combinators.hpp>
#include <system_error>

TEST_CASE("Test read_file") {
    REQUIRE(imp::read_file("test/test_files/hello.txt") == "Hello, world!\n");

    // This test requires that imp::read_file throws an error when encountering
    // a file that doesn't exist
    REQUIRE_THROWS_AS(
        imp::read_file("this file does not exist.txt"),
        std::system_error);
}

TEST_CASE("Tests set up correctly") { REQUIRE(true); }
