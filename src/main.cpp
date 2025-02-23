#include "../includes/MatchingEngine.hpp"
#include "../includes/OrderBook.hpp"
#include <iostream>

int main() {
  // Create the matching engine
  MatchingEngine matchingEngine = MatchingEngine();

  // Add orders to the order book
  OrderBook::getOrderBook()->addOrder(
      new LimitOrder(1, 101, true, 100.0, 50, 1));
  OrderBook::getOrderBook()->addOrder(
      new LimitOrder(2, 102, true, 100.0, 100, 2));
  OrderBook::getOrderBook()->addOrder(
      new LimitOrder(3, 103, true, 100.0, 150, 3));
  OrderBook::getOrderBook()->addOrder(
      new LimitOrder(4, 201, false, 100.0, 80, 4));
  OrderBook::getOrderBook()->addOrder(
      new LimitOrder(5, 202, false, 100.0, 120, 5));

  // Print the order book
  matchingEngine.printOrderBook();

  // Execute the matching engine
  matchingEngine.executeMatchingEngine();

  // Print the order book
  matchingEngine.printOrderBook();

  return 0;
}
