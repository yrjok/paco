#pragma once

#include <paco/recognizer_base.h>

#include <string_view>
#include <functional>

namespace paco {

namespace fundamentals {

class empty : public recognizer_base {
public:
  match matches (std::string_view const content) const override;

  std::unique_ptr<recognizer_base> clone () const override;
};

class any : public recognizer_base {
public:
  match matches (std::string_view const content) const override;

  std::unique_ptr<recognizer_base> clone () const override;
};

} // fundamentals

} // ns paco
