#include <gtest/gtest.h>

#include "../includes/LimitOrder.hpp"
#include "../includes/MarketOrder.hpp"
#include "../includes/OrderBook.hpp"
#include "../includes/StopOrder.hpp"

// Test cases
TEST(OrderBookTest, AddingLimitOrder) {

  // Create an order book
  OrderBook orderBook = OrderBook();

  // Create a Limit Order
  LimitOrder limitOrder = LimitOrder(1, 101, true, 101.0, 50, 1);

  // Add the Limit Order to the Order Book
  orderBook.addOrder(&limitOrder);

  // Check if the Limit Order was added to the Order Book
  EXPECT_EQ(orderBook.buyOrders[101.0].front().orderID, 1);
}

TEST(OrderBookTest, AddingMarketOrder) {

  // Create an order book
  OrderBook orderBook = OrderBook();

  // Create a Market Order
  MarketOrder marketOrder = MarketOrder(6, 301, true, 100, 6);

  // Add the Market Order to the Order Book
  orderBook.addOrder(&marketOrder);

  // Check if the Market Order was added to the Order Book
  EXPECT_EQ(orderBook.marketBuyOrders.front().orderID, 6);
}

// Main function for running tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
