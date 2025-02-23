#ifndef MARKETORDER_HPP
#define MARKETORDER_HPP

#include "Order.hpp"

struct MarketOrder : public Order {

  // Default constructor
  MarketOrder() = default;

  // Constructor with initialization list
  MarketOrder(int orderID, int traderID, bool isBuy, int quantity,
              long timestamp)
      : Order(orderID, traderID, isBuy, quantity, timestamp) {}
};

#endif // MARKETORDER_HPP