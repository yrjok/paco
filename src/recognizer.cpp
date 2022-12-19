#include <paco/recognizer.h>

#include <paco/fundamental.h>
#include <paco/combinators.h>
#include <paco/recognizer_base.h>

#include <utility>

namespace paco {

recognizer::recognizer ()
  : impl_(std::make_unique<empty>())
{ }

recognizer::recognizer (std::unique_ptr<recognizer_base> impl)
  : impl_(std::move(impl))
{ }

recognizer::recognizer (recognizer const & other)
  : impl_(other.impl_->clone())
{ }

recognizer::recognizer (recognizer && other)
  : impl_(std::move(other.impl_))
{ }

recognizer & recognizer::operator=(recognizer const & other) {
  impl_ = other.impl_->clone();
  return *this;
}

recognizer & recognizer::operator=(recognizer && other) {
  using std::swap;
  swap(impl_, other.impl_);
  return *this;
}

recognizer::~recognizer () { }

recognizer::recognizer (recognizer_base const & impl)
  : recognizer(impl.clone())
{ }

recognizer & recognizer::operator= (recognizer_base const & other) {
  impl_ = other.clone();
  return *this;
}

recognizer::operator recognizer_base const & () {
  return *impl_;
}

auto recognizer::matches (std::string_view const content) const -> match {
  return impl_->matches(content);
}

recognizer_base const & recognizer::impl () const {
  return *impl_;
}

recognizer operator|| (recognizer const & lhs, recognizer const & rhs) {
  return either(lhs, rhs);
}

recognizer operator&& (recognizer const & lhs, recognizer const & rhs) {
  return sequence(lhs, rhs);
}

recognizer operator! (recognizer const & other) {
  return negate(other);
}

} // ns paco
