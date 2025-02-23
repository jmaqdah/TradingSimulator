#include "../includes/OrderBook.hpp"

// Define any static variables and functions
OrderBook *OrderBook::instance = nullptr;

OrderBook *OrderBook::getOrderBook() {

  if (OrderBook::instance == nullptr) {
    OrderBook::instance = new OrderBook();
  }

  return OrderBook::instance;
}

void OrderBook::addOrder(LimitOrder order) {
  if (order.isBuy) {
    OrderBook::getOrderBook()->buyOrders[order.price].push_back(order);
  } else {
    OrderBook::getOrderBook()->sellOrders[order.price].push_back(order);
  }
}

OrderBook::OrderBook() {}

OrderBook::~OrderBook() { delete OrderBook::instance; }