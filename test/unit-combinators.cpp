#include <doctest/doctest.h>

#include <paco/fundamentals.h>
#include <paco/combinators.h>

#include <string>
#include <vector>

using namespace paco;

TEST_SUITE("Combinators") {
  TEST_CASE("Either") {
    auto base = std::make_unique<fundamentals::any>();
    satisfying first(base->clone(), [](auto v) { return v == "a"; });
    satisfying second(base->clone(), [](auto v) { return v == "b"; });

    either recognizer(first.clone(), second.clone());

    SUBCASE("Matches using the first recognizer") {
      std::string content("abcd");
      REQUIRE(first.matches(content));
      REQUIRE(not second.matches(content));
      CHECK(recognizer.matches(content));
    }

    SUBCASE("Matches using the second recognizer") {
      std::string content("bcd");
      REQUIRE(not first.matches(content));
      REQUIRE(second.matches(content));
      CHECK(recognizer.matches(content));
    }

    SUBCASE("Does not match anything else") {
      std::string content("cd");
      REQUIRE(not first.matches(content));
      REQUIRE(not second.matches(content));
      CHECK(not recognizer.matches(content));
    }
  }

  TEST_CASE("Sequence") {
    auto base = std::make_unique<fundamentals::any>();
    satisfying first(base->clone(), [](auto v) { return v == "a"; });
    satisfying second(base->clone(), [](auto v) { return v == "b"; });

    sequence recognizer(first.clone(), second.clone());

    SUBCASE("Matches sequences") {
      std::string content("ab");
      REQUIRE(first.matches(content));
      REQUIRE(second.matches(content.substr(1, 1)));
      CHECK(recognizer.matches(content));
    }

    SUBCASE("Does not match part of the sequence") {
      std::string content("ac");
      REQUIRE(first.matches(content));
      REQUIRE(not second.matches(content.substr(1, 1)));
      CHECK(not recognizer.matches(content));
    }

    SUBCASE("Does not match anything else") {
      std::string content("def");
      REQUIRE(not first.matches(content));
      REQUIRE(not second.matches(content.substr(1, 1)));
      CHECK(not recognizer.matches(content));
    }
  }

  TEST_CASE("Many") {
    auto base = std::make_unique<fundamentals::any>();
    satisfying first(base->clone(), [](auto v) { return v == "y"; });
    many recognizer(first.clone());

    SUBCASE("Matches a sequences of as many matching parts as possible") {
      std::string content("yyyyyba");
      REQUIRE(first.matches(content));
      auto match = recognizer.matches(content);
      CHECK(match.has_value());
      CHECK(match.value().size() == 5);
    }

    SUBCASE("Matches only the first uninterrupted sequence.") {
      std::string content("yyy yyyyyba");
      REQUIRE(first.matches(content));
      auto match = recognizer.matches(content);
      CHECK(match.has_value());
      CHECK(match.value().size() == 3);
    }
  }

  TEST_CASE("Negate") {
    auto base = std::make_unique<fundamentals::any>();
    satisfying negated(base->clone(), [](auto v) { return v == "a"; });
    negate recognizer(negated);

    SUBCASE("Returns size one match if the negated recognizer fails.") {
      std::string content("sheep");
      REQUIRE(not negated.matches(content));
      auto match = recognizer.matches(content);
      CHECK(match.has_value());
      CHECK(match.value().size() == 1);
    }

    SUBCASE("Does not match if the negated recognizer succeeds.") {
      std::string content("abba");
      REQUIRE(negated.matches(content));
      auto match = recognizer.matches(content);
      CHECK(not match.has_value());
    }
  }
}
