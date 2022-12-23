#include <paco/common.h>
#include <paco/recognizer.h>

#include <doctest/doctest.h>

using namespace paco;

TEST_SUITE("Recognizer operations") {
  TEST_CASE("Chaining") {
    auto matcher = recognizers::number().then(recognizers::letter());

    SUBCASE("matches the expected sequence") {
      std::string const text("4a");
      CHECK(matcher.matches(text));
    }
  }

  TEST_CASE("Negation") {
    auto matcher = not recognizers::letter();

    SUBCASE("matches a single other character") {
      std::string const text(".123");
      auto match = matcher.matches(text);
      CHECK(match.has_value());
      CHECK_EQ(match.value().size, 1);
    }
  }

  TEST_CASE("Disjunction") {
    auto matcher = recognizers::uppercase() or recognizers::numeral();

    SUBCASE("matches if either one matches") {
      std::string const first("D9");
      CHECK(matcher.matches(first));
      std::string const second("9D");
      CHECK(matcher.matches(second));
    }
  }
}
