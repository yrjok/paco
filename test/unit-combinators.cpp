#include <doctest/doctest.h>

#include <paco/fundamentals.h>
#include <paco/combinators.h>

#include <string>
#include <vector>

using namespace paco;

TEST_SUITE("Combinators") {
  TEST_CASE("Satisying") {
    auto base = fundamentals::any();
    auto predicate = [](std::string_view v) {
      return v.size() == 0 or v.front() >= 'a' and v.front() <= 'z';
    };
    auto matcher = satisfying(base, predicate);

    SUBCASE("Matches when both the underlying matcher and the predicate matches") {
      std::string text = "word";
      REQUIRE(base.matches(text));
      REQUIRE(predicate(text));
      CHECK(matcher.matches(text));
    }

    SUBCASE("does not match when the underlying matcher does not match") {
      std::string text = "";
      REQUIRE(not base.matches(text));
      REQUIRE(predicate(text));
      CHECK(not matcher.matches(text));
    }

    SUBCASE("does not match when the predicate does not match") {
      std::string text = "WORD";
      REQUIRE(base.matches(text));
      REQUIRE(not predicate(text));
      CHECK(not matcher.matches(text));
    }
  }

  TEST_CASE("Either") {
    auto base = fundamentals::any();
    auto low = [](std::string_view v) { return v.front() >= 'a' and v.front() <= 'b'; };
    auto high = [](std::string_view v) { return v.front() >= 'b' and v.front() <= 'c'; };
    satisfying first(base, low);
    satisfying second(base, high);

    either recognizer(first.clone(), second.clone());

    SUBCASE("Matches when only the first recognizer matches") {
      std::string content("alice");
      REQUIRE(first.matches(content));
      REQUIRE(not second.matches(content));
      CHECK(recognizer.matches(content));
    }

    SUBCASE("Matches when only the second recognizer matches") {
      std::string content("combine");
      REQUIRE(not first.matches(content));
      REQUIRE(second.matches(content));
      CHECK(recognizer.matches(content));
    }

    SUBCASE("Does not match when none of both match") {
      std::string content(":)");
      REQUIRE(not first.matches(content));
      REQUIRE(not second.matches(content));
      CHECK(not recognizer.matches(content));
    }

    SUBCASE("Matches when both match") {
      std::string content("bob");
      REQUIRE(first.matches(content));
      REQUIRE(second.matches(content));
      CHECK(recognizer.matches(content));
    }
  }

  TEST_CASE("Sequence") {
    auto base = fundamentals::any();
    satisfying first(base, [](auto v) { return v == "a"; });
    satisfying second(base, [](auto v) { return v == "b"; });

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
    auto base = fundamentals::any();
    satisfying first(base, [](auto v) { return v == "y"; });
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
