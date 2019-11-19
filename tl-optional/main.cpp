#define CATCH_CONFIG_MAIN 
#include "catch2/catch.hpp"
#include "tl/optional.hpp"

#include <optional>
#include <string>

TEST_CASE("Bassic test")
{
    tl::optional<char> shouldBeEmpty;
    REQUIRE_FALSE(shouldBeEmpty);

    tl::optional<char> shouldBeA {'A'};
    REQUIRE(shouldBeA);
    REQUIRE('A' == *shouldBeA);
}

TEST_CASE("map")
{
    tl::optional<std::string> empty;

    auto shouldBeEmpty {empty.map(&std::string::size)};
    REQUIRE_FALSE(shouldBeEmpty);

    tl::optional<std::string> someData {"Some data"};

    auto shouldBe9 {someData.map(&std::string::size)};
    REQUIRE(shouldBe9);
    REQUIRE(9 == *shouldBe9);
}

tl::optional<int> ToInt(const std::string& input)
{
    return std::stoi(input);
}

TEST_CASE("and_then")
{
    tl::optional<std::string> empty;
    auto shouldBeEmpty {empty.and_then(ToInt)};
    REQUIRE_FALSE(shouldBeEmpty);

    tl::optional<std::string> someData {"10"};
    auto shouldBe10 {someData.and_then(ToInt)};
    REQUIRE(shouldBe10);
    REQUIRE(10 == *shouldBe10);
}