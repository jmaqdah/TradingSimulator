#ifndef STOPORDER_HPP
#define STOPORDER_HPP

#include "Order.hpp"

struct StopOrder : public Order {

  double stopPrice;

  // Default constructor
  StopOrder() = default;

  // Constructor with initialization list
  StopOrder(int orderID, int traderID, bool isBuy, double stopPrice,
            int quantity, long timestamp)
      : Order(orderID, traderID, isBuy, quantity, timestamp),
        stopPrice(stopPrice) {}
};

#endif // STOPORDER_HPP
