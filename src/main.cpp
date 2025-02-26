#include "../includes/MatchingEngine.hpp"
#include "../includes/OrderBook.hpp"
#include <iostream>

int main() {
  // Create the matching engine
  MatchingEngine matchingEngine = MatchingEngine(new OrderBook());

  // Add Limit orders to the order book
  matchingEngine.addOrder(new LimitOrder(1, 101, true, 101.0, 50, 1));
  matchingEngine.addOrder(new LimitOrder(2, 102, true, 102.0, 100, 2));
  matchingEngine.addOrder(new LimitOrder(3, 103, true, 103.0, 150, 3));
  matchingEngine.addOrder(new LimitOrder(4, 201, false, 104.0, 80, 4));
  matchingEngine.addOrder(new LimitOrder(5, 202, false, 105.0, 120, 5));

  std::cout << "---------------------\n";

  // Print the order book
  matchingEngine.printOrderBook();

  std::cout << "---------------------\n";

  // Add Stop orders to the order book
  matchingEngine.addOrder(new StopOrder(7, 401, true, 105.0, 50, 7));

  // Add Market orders to the order book
  matchingEngine.addOrder(new MarketOrder(6, 301, true, 100, 6));

  std::cout << "---------------------\n";

  // Print the order book
  matchingEngine.printOrderBook();

  std::cout << "---------------------\n";

  return 0;
}
