#include <catch2/catch_test_macros.hpp>
#include <imp/read_file.hpp>

TEST_CASE("Test read_file") {
    REQUIRE(imp::read_file("test/test_files/hello.txt") == "Hello, world!\n");
}
TEST_CASE("Tests set up correctly") {
    REQUIRE(true);
}
