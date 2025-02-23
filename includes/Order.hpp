#ifndef ORDER_HPP
#define ORDER_HPP

struct Order {
  int orderID;
  int traderID;
  bool isBuy;
  int quantity;
  long timestamp;

  Order() = default;

  Order(int orderID, int traderID, bool isBuy, int quantity, long timestamp)
      : orderID(orderID), traderID(traderID), isBuy(isBuy), quantity(quantity),
        timestamp(timestamp) {}

  virtual ~Order() = default;
};

#endif // ORDER_HPP