#include <paco/fundamentals.h>

#include <optional>
#include <functional>
#include <utility>

namespace paco {

namespace fundamentals {

auto empty::matches (std::string_view const content) const -> match {
  return content.empty() ? match(content) : std::nullopt;
}

std::unique_ptr<recognizer_base> empty::clone () const {
  return std::make_unique<empty>();
}

auto any::matches (std::string_view const content) const -> match {
  return content.empty() ? std::nullopt : match(content.substr(0, 1));
}

std::unique_ptr<recognizer_base> any::clone () const {
  return std::make_unique<any>();
}

} // ns fundamentals

} // ns paco
