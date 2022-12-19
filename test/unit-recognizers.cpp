#include <doctest/doctest.h>

#include <paco/fundamentals.h>

#include <string>
#include <vector>

using namespace paco;

TEST_SUITE("Recognizers") {
  TEST_CASE("Empty") {
    empty recognizer;

    SUBCASE("Matches the empty string") {
      std::string empty_string("");
      auto result = recognizer.matches(empty_string);
      CHECK(result.has_value());
      CHECK_EQ(result.value(), empty_string);
    }

    SUBCASE("Does not match anything else") {
      std::vector<std::string> others { "  ", "\n", "a word", "c" };
      for (std::string const & s : others) {
        auto result = recognizer.matches(s);
        CHECK(not result.has_value());
      }
    }
  }

  TEST_CASE("Any") {
    any recognizer;
    SUBCASE("Matches any character") {
      std::string content("abcd");
      auto result = recognizer.matches(content);
      CHECK(result.has_value());
      CHECK_EQ(result.value(), content.substr(0, 1));
    }

    SUBCASE("Does not match the empty string") {
      std::string empty_string("");
      auto result = recognizer.matches(empty_string);
      CHECK(not result.has_value());
    }
  }

  TEST_CASE("Satisfying") {
    auto func = [](std::string_view const view) { return view.at(0) == 'a'; };
    satisfying recognizer(std::make_unique<any>(), func);

    SUBCASE("Matches character for which the predicate is true") {
      std::string content("abba");
      auto result = recognizer.matches(content);
      CHECK(result.has_value());
      CHECK_EQ(result.value(), "a");
    }

    SUBCASE("Does not match characters for which the predicate is false") {
      std::string content("baab");
      auto result = recognizer.matches(content);
      CHECK(not result.has_value());
    }
  }
}
