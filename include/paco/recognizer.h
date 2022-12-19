#pragma once

#include <memory>
#include <optional>
#include <string_view>

namespace paco {

// Forward declarations
class recognizer_base;

class recognizer {
public:
  // Helper types
  using match = std::optional<std::string_view>;

  // Functions
  recognizer ();
  recognizer (std::unique_ptr<recognizer_base> impl);
  recognizer (recognizer const & other);
  recognizer (recognizer && other);
  recognizer & operator=(recognizer const & other);
  recognizer & operator=(recognizer && other);
  ~recognizer ();

  // Conversions
  /* implicit */ recognizer (recognizer_base const & impl);
  recognizer & operator= (recognizer_base const & other);
  operator recognizer_base const & ();

  match matches (std::string_view const content) const;

  recognizer_base const & impl () const;
private:
  // Members
  std::unique_ptr<recognizer_base> impl_;
};

recognizer operator|| (recognizer const & lhs, recognizer const & rhs);
recognizer operator&& (recognizer const & lhs, recognizer const & rhs);
recognizer operator! (recognizer const & other);

} // ns paco