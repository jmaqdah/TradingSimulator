# 1. Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# 2. Add test executable
add_executable(OrderBookTests ./test_order_book.cpp ../src/OrderBook.cpp)

# 3. Link with GTest and pthread
target_link_libraries(OrderBookTests GTest::GTest GTest::Main pthread)

# 4. Register tests
add_test(NAME OrderBookAddOrders COMMAND OrderBookTests)
