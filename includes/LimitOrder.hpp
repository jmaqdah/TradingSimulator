#ifndef LIMITORDER_HPP
#define LIMITORDER_HPP

#include "Order.hpp"

struct LimitOrder : public Order {

  double price;

  // Default constructor
  LimitOrder() = default;

  // Constructor with initialization list
  LimitOrder(int orderID, int traderID, bool isBuy, double price, int quantity,
             long timestamp)
      : Order(orderID, traderID, isBuy, quantity, timestamp), price(price) {}
};

#endif // LIMITORDER_HPP