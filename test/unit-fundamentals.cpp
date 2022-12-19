#include <paco/fundamentals.h>

#include <string>
#include <string_view>
#include <vector>

#include <doctest/doctest.h>

using namespace paco;

TEST_SUITE("Fundamentals") {
  TEST_CASE("empty") {
    auto recognizer = fundamentals::empty();

    SUBCASE("Matches the empty string") {
      std::string empty_string("");
      auto result = recognizer.matches(empty_string);
      CHECK(result.has_value());
      CHECK_EQ(result.value(), empty_string);
    }

    SUBCASE("Does not match anything else") {
      for (unsigned val = 0; val < 256; ++val) {
        auto character = static_cast<char>(val);
        std::string text(1, character);
        CHECK(not recognizer.matches(text));
      }

      std::vector<std::string> others { "  ", "\n", "a word", "c" };
      for (std::string const & s : others) {
        auto result = recognizer.matches(s);
        CHECK(not result.has_value());
      }
    }
  }

  TEST_CASE("Any") {
    auto recognizer = fundamentals::any();

    SUBCASE("Matches any character") {
      for (unsigned val = 0; val < 256; ++val) {
        auto character = static_cast<char>(val);
        std::string text(1, character);

        auto result = recognizer.matches(text);
        CHECK(result.has_value());
        CHECK_EQ(result.value(), text.substr(0, 1));
      }
    }

    SUBCASE("Does not match the empty string") {
      std::string empty_string("");
      CHECK(not recognizer.matches(empty_string).has_value());
    }
  }
}
