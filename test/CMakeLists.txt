include(FetchContent)

FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest
  GIT_TAG v2.4.9
)

FetchContent_MakeAvailable(doctest)

add_executable(run_tests)
target_include_directories(run_tests PRIVATE ${doctest_SOURCE_DIR})
target_sources(run_tests
  PRIVATE
    test_runner.cpp
    #unit-fundamentals.cpp
    unit-recognizers.cpp
    #unit-combinators.cpp
    #unit-recognizer_factory.cpp
)
target_link_libraries(run_tests PRIVATE paco)