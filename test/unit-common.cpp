#include <paco/common.h>

#include <doctest/doctest.h>

using namespace paco;

TEST_SUITE("Common matchers") {
  TEST_CASE("number") {
    auto matcher = recognizers::number();

    SUBCASE("matches integral numbers") {
      std::string const text("42");
      CHECK(matcher.matches(text));
    }

    SUBCASE("does not match signs") {
      std::string const text("-255");
      CHECK(not matcher.matches(text));
    }

    SUBCASE("does not include punctuation") {
      std::string const text("123.456");
      auto match = matcher.matches(text);
      CHECK(match.has_value());
      CHECK_EQ(match.value(), "123");
    }
  }

  TEST_CASE("string literal") {
    std::string const literal("paco");
    auto matcher = recognizers::literal(literal);

    SUBCASE("matches the literal") {
      CHECK(matcher.matches(literal));
    }
    
    SUBCASE("matches the literal without suffix if there is a suffix") {
      std::string text = literal + "ok";
      auto match = matcher.matches(text);
      CHECK(match.has_value());
      CHECK_EQ(match.value().size(), literal.size());
    }
  }
}

