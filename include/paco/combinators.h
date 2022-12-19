#pragma once

#include <paco/recognizer_base.h>
#include <paco/recognizer.h>

#include <functional>
#include <memory>
#include <string_view>

namespace paco {

/**
 * Matches if either the first or the second matcher matches.
 */
class either : public recognizer_base {
public:
  either (pointer first, pointer second);
  either (recognizer first, recognizer second);

  match matches (std::string_view content) const override;

  pointer clone () const override;

private:
  match match_first (std::string_view content) const;
  match match_second (std::string_view content) const;

  pointer first_;
  pointer second_;
};

/**
 * Matches if and only if the second matcher matches on whatever remains after
 * succesfully matching the first matcher.
 */
class sequence : public recognizer_base {
public:
  sequence (pointer first, pointer second);
  sequence (recognizer first, recognizer second);

  match matches (std::string_view content) const override;

  pointer clone () const override;

private:
  match match_first (std::string_view content) const;
  match match_second (std::string_view content) const;

  pointer first_;
  pointer second_;
};

/**
 * Matches as many sequential elements as possible.
 */
class many : public recognizer_base {
public:
  many (pointer elem);
  many (recognizer elem);

  match matches (std::string_view content) const override;

  pointer clone () const override;

private:
  pointer elem_;
};

/**
 * Returns an size one match if the negated matches does NOT match.
 */
class negate : public recognizer_base {
public:
  negate (pointer negated);
  negate (recognizer negated);

  match matches (std::string_view content) const override;

  pointer clone () const override;

private:
  pointer negated_;
};

/**
 * Matches if the match produced by the given mather also satisfies a predicate.
*/
class satisfying : public recognizer_base {
public:
  using predicate = std::function<bool(std::string_view)>;

  satisfying (std::unique_ptr<recognizer_base> other, predicate pred);
  satisfying (recognizer_base const & other, predicate pred);

  match matches (std::string_view const content) const override;

  std::unique_ptr<recognizer_base> clone () const override;

private:
  std::unique_ptr<recognizer_base> other_;
  predicate pred_;
};

} // ns paco
