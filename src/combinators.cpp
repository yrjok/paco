#include <paco/combinators.h>

#include <cassert>
#include <functional>

namespace paco {

either::either (pointer first, pointer second)
  : first_(std::move(first))
  , second_(std::move(second))
{ }

either::either (recognizer first, recognizer second)
  : either(first.impl().clone(), second.impl().clone())
{ }

auto either::matches (std::string_view const content) const -> match {
  auto result = match_first(content);
  if (result) { return result; }
  return match_second(content);
}

auto either::clone () const -> pointer {
  return std::make_unique<either>(first_->clone(), second_->clone());
}

auto either::match_first (std::string_view const content) const -> match {
  return first_->matches(content);
}

auto either::match_second (std::string_view const content) const -> match {
  return second_->matches(content);
}

sequence::sequence (pointer first, pointer second)
  : first_(std::move(first))
  , second_(std::move(second))
{ }

sequence::sequence (recognizer first, recognizer second)
  : sequence(first.impl().clone(), second.impl().clone())
{ }

auto sequence::matches (std::string_view const content) const -> match {
  auto first_match = match_first(content);
  if (first_match) {
    auto remainder = content;
    remainder.remove_prefix(first_match->size());
    auto second_match = match_second(remainder);
    if (second_match) {
      return content.substr(0, first_match->size() + second_match->size());
    }
  }
  return std::nullopt;
}

auto sequence::clone () const -> pointer {
  return std::make_unique<sequence>(first_->clone(), second_->clone());
}

auto sequence::match_first (std::string_view content) const -> match {
  return first_->matches(content);
}

auto sequence::match_second (std::string_view content) const -> match {
  return second_->matches(content);
}

many::many (pointer elem)
  : elem_(std::move(elem))
{ }

many::many (recognizer elem)
  : many(elem.impl().clone())
{ }

auto many::matches (std::string_view const content) const -> match {
  std::size_t total_size = 0;
  auto remainder = content;
  while (auto matched_element = elem_->matches(remainder)) {
    assert(matched_element->size() > 0);
    total_size += matched_element->size();
    remainder.remove_prefix(matched_element->size());
  }
  if (total_size > 0) {
    return content.substr(0, total_size);
  } else {
    return std::nullopt;
  }
}

auto many::clone () const -> pointer {
  return std::make_unique<many>(elem_->clone());
}

negate::negate (pointer negated)
  : negated_(std::move(negated))
{ }

negate::negate (recognizer negated)
  : negate(negated.impl().clone())
{ }

auto negate::matches (std::string_view content) const -> match {
  if (negated_->matches(content)) { return std::nullopt; }
  return content.substr(0, 1);
}

auto negate::clone () const -> pointer {
  return std::make_unique<negate>(negated_->clone());
}

satisfying::satisfying (std::unique_ptr<recognizer_base> other, predicate pred)
  : other_(std::move(other))
  , pred_(std::move(pred))
{ }

satisfying::satisfying (recognizer_base const & other, predicate pred)
  : other_(other.clone())
  , pred_(std::move(pred))
{ }

auto satisfying::matches (std::string_view const content) const -> match {
  auto result = other_->matches(content);
  if (result.has_value() and pred_(result.value())) {
    return result;
  }
  return std::nullopt;
}

std::unique_ptr<recognizer_base> satisfying::clone () const {
  return std::make_unique<satisfying>(other_->clone(), pred_);
}


} // ns paco
