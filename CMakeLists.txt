cmake_minimum_required(VERSION 3.10)
project(StockExchange)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
include_directories(external src)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(COPY web DESTINATION ${CMAKE_BINARY_DIR})
add_executable(stock_exchange ${SOURCES})

# Google Test setup
include(FetchContent)
fetchcontent_declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON)
fetchcontent_makeavailable(googletest)

enable_testing()

# Test executable
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
file(GLOB_RECURSE LIB_SOURCES
    src/models/User.cpp
    src/engine/MatchingEngine.cpp
    src/services/PortfolioManager.cpp
    src/valuation/PortfolioValuation.cpp
    src/api/ApiServer.cpp
)
add_executable(run_tests ${TEST_SOURCES} ${LIB_SOURCES})
target_link_libraries(run_tests GTest::gtest_main pthread)
target_include_directories(run_tests PRIVATE external src)

include(GoogleTest)

# Run tests directly instead of using gtest_discover_tests
add_test(NAME AllTests COMMAND run_tests)