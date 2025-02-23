#ifndef LIMITORDER_HPP
#define LIMITORDER_HPP

struct LimitOrder {

  int orderID;
  int traderID;
  bool isBuy;
  double price;
  int quantity;
  long timestamp;

  // Default constructor
  LimitOrder() = default;

  // Constructor with initialization list
  LimitOrder(int orderID, int traderID, bool isBuy, double price, int quantity,
             long timestamp)
      : orderID(orderID), traderID(traderID), isBuy(isBuy), price(price),
        quantity(quantity), timestamp(timestamp) {}
};

#endif // LIMITORDER_HPP