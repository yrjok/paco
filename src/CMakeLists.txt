add_library(paco STATIC)

target_sources(paco
  PRIVATE
    recognizer.cpp
    fundamentals.cpp
    combinators.cpp
    common.cpp
)
