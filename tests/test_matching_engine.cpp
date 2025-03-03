#include <gtest/gtest.h>

#include "../includes/LimitOrder.hpp"
#include "../includes/MarketOrder.hpp"
#include "../includes/MatchingEngine.hpp"
#include "../includes/OrderBook.hpp"
#include "../includes/StopOrder.hpp"

// Test cases
TEST(MatchingEngineTest, MatchLimitOrders) {
  // Create a matching engine
  OrderBook *orderBook = new OrderBook();
  MatchingEngine matchingEngine = MatchingEngine(orderBook);

  // Add Limit orders to the order book
  matchingEngine.addOrder(new LimitOrder(1, 101, true, 101.0, 50, 1));
  matchingEngine.addOrder(new LimitOrder(2, 102, true, 101.0, 100, 2));
  matchingEngine.addOrder(new LimitOrder(3, 103, true, 103.0, 150, 3));
  matchingEngine.addOrder(new LimitOrder(4, 201, false, 101.0, 80, 4));
  matchingEngine.addOrder(new LimitOrder(5, 202, false, 103.0, 200, 5));

  // Check if orders were matched correctly
  EXPECT_EQ(matchingEngine.getOrderBook()->buyOrders[101.0].front().quantity,
            23);

  EXPECT_EQ(matchingEngine.getOrderBook()->buyOrders[101.0].back().quantity,
            47);

  // Check buy orders for price 103 is empty
  EXPECT_TRUE(matchingEngine.getOrderBook()->buyOrders[103.0].empty());

  // Check sell orders for price 101.0 is empty
  EXPECT_TRUE(matchingEngine.getOrderBook()->sellOrders[101.0].empty());

  // Check sell orders for price 103.0 is has quantity 50
  EXPECT_EQ(matchingEngine.getOrderBook()->sellOrders[103.0].front().quantity,
            50);
}

// Main function for running tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}