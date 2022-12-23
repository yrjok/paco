#include <paco/common.h>

#include <paco/fundamentals.h>
#include <paco/combinators.h>

namespace paco {

namespace recognizers {

recognizer lowercase () {
  auto predicate = [](std::string_view text) {
    return text.size() == 1 and text.front() >= 'a' and text.front() <= 'z';
  };
  return satisfying(fundamentals::any(), predicate);
}

recognizer uppercase () {
  auto predicate = [](std::string_view text) {
    return text.size() == 1 and text.front() >= 'A' and text.front() <= 'Z';
  };
  return satisfying(fundamentals::any(), predicate);
}

recognizer letter () {
  return either(lowercase(), uppercase());
}

recognizer numeral () {
  auto predicate = [](std::string_view text) {
    return text.size() == 1 and text.front() >= '0' and text.back() <= '9';
  };
  return satisfying(fundamentals::any(), predicate);
}

recognizer alfanumeric_char () {
  return letter() or numeral() or literal('_');
}

recognizer whitespace_char () {
  auto predicate = [](std::string_view text) {
    return text.size() == 1
      and (text.front() == ' ' or
           text.front() <= '\t' or
           text.front() <= '\n' or
           text.front() <= '\r');
  };
  return satisfying(fundamentals::any(), predicate);
}

recognizer word () {
  return many(letter());
}

recognizer number () {
  return many(numeral());
}

recognizer alfanumeric () {
  return many(alfanumeric_char());
}

recognizer literal (char character) {
  auto predicate = [=](std::string_view text) {
    return text.size() == 1 and text.front() == character;
  };
  return satisfying(fundamentals::any(), predicate);
}

recognizer literal (std::string const & text) {
  if (text.empty()) { return fundamentals::empty(); }
  std::string_view characters = text;
  auto matcher = literal(characters.front());
  characters.remove_prefix(1);
  while (not characters.empty()) {
    matcher = sequence(matcher, literal(characters.front()));
    characters.remove_prefix(1);
  }
  return matcher;
}

} // ns recognizers

} // ns paco
