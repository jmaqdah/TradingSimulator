# 1. Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# 2. Add test executable
add_executable(OrderBookTests ./test_order_book.cpp ../src/OrderBook.cpp)
add_executable(MatchingEngineTests ./test_matching_engine.cpp ../src/OrderBook.cpp ../src/MatchingEngine.cpp)

# 3. Link with GTest and pthread
target_link_libraries(OrderBookTests GTest::GTest GTest::Main pthread)
target_link_libraries(MatchingEngineTests GTest::GTest GTest::Main pthread)

# 4. Register tests
add_test(NAME OrderBookTests COMMAND OrderBookTests)
add_test(NAME MatchingEngineTests COMMAND MatchingEngineTests)