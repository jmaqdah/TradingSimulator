#include "../includes/MatchingEngine.hpp"
#include "../includes/OrderBook.hpp"

#include <iostream>

MatchingEngine::~MatchingEngine() { delete m_orderBook; }

void MatchingEngine::printOrderBook() {

  std::cout << "- Limit Buy Orders:\n";
  for (const auto &buyOrder : m_orderBook->buyOrders) {
    std::cout << "    Price: $" << buyOrder.first << "\n";
    for (const auto &order : buyOrder.second) {
      std::cout << "      Quantity: " << order.quantity << "\n";
    }
  }

  std::cout << "- Market Buy Orders:\n";
  for (const auto &buyOrder : m_orderBook->marketBuyOrders) {
    std::cout << "    Quantity: " << buyOrder.quantity << "\n";
  }

  std::cout << "- Limit Sell Orders:\n";
  for (const auto &sellOrder : m_orderBook->sellOrders) {
    std::cout << "    Price: $" << sellOrder.first << "\n";
    for (const auto &order : sellOrder.second) {
      std::cout << "      Quantity: " << order.quantity << "\n";
    }
  }

  std::cout << "- Market Sell Orders:\n";
  for (const auto &sellOrder : m_orderBook->marketSellOrders) {
    std::cout << "    Quantity: " << sellOrder.quantity << "\n";
  }

  std::cout << "- Stop Orders:\n";
  for (const auto &stopOrder : m_orderBook->stopOrders) {
    std::cout << "    Price: $" << stopOrder.stopPrice << "\n";
    std::cout << "    Quantity: " << stopOrder.quantity << "\n";
  }
}

void MatchingEngine::processMarketOrders() {
  // Loop over market buy orders and match with the lowest sell orders
  while (!m_orderBook->marketBuyOrders.empty() &&
         !m_orderBook->sellOrders.empty()) {

    auto &marketBuyOrder = m_orderBook->marketBuyOrders.front();
    auto &sellQueue = m_orderBook->sellOrders.begin()->second;

    // Update the last traded price
    setLastTradedPrice(m_orderBook->sellOrders.begin()->first);
    // We pass false to prevent the execution of market orders recursively, we
    // just want to add to the orderbook. Note: current market orders take
    // precedence over newly created market orders from stop orders
    processStopOrders(false);

    // Match the market buy order with the lowest sell orders
    matchMarketOrder(marketBuyOrder, sellQueue);

    // If we exhaust the sell queue, remove it and move to the next price
    if (sellQueue.empty())
      m_orderBook->sellOrders.erase(m_orderBook->sellOrders.begin());

    // If after matching the market buy order quantity is 0, remove it
    if (marketBuyOrder.quantity == 0)
      m_orderBook->marketBuyOrders.pop_front();
  }

  // Loop over market sell orders and match with the highest buy orders
  while (!m_orderBook->marketSellOrders.empty() &&
         !m_orderBook->buyOrders.empty()) {

    auto &marketSellOrder = m_orderBook->marketSellOrders.front();
    auto &buyQueue = m_orderBook->buyOrders.rbegin()->second;

    // Update the last traded price
    setLastTradedPrice(m_orderBook->buyOrders.begin()->first);

    // Match the market sell order with the highest buy orders
    matchMarketOrder(marketSellOrder, buyQueue);

    // If we exhaust the buy queue, remove it and move to the next price
    if (buyQueue.empty())
      m_orderBook->buyOrders.erase(m_orderBook->buyOrders.rbegin()->first);

    // If after matching the market sell order quantity is 0, remove it
    if (marketSellOrder.quantity == 0)
      m_orderBook->marketSellOrders.pop_front();
  }
}

void MatchingEngine::matchMarketOrder(MarketOrder &marketOrder,
                                      std::deque<LimitOrder> &orderQueue) {
  // Loop over the order queue and match the market order
  for (auto it = orderQueue.begin();
       it != orderQueue.end() && marketOrder.quantity > 0;) {

    // Get the matched quantity
    int matchQty = std::min(marketOrder.quantity, it->quantity);

    // Subtract the matched quantity from the market order and the limit order
    marketOrder.quantity -= matchQty;
    it->quantity -= matchQty;

    // Remove the limit order if it is fully matched then point to the next
    if (it->quantity == 0) {
      it = orderQueue.erase(it);
    } else
      ++it;
  }
}

void MatchingEngine::processStopOrders(bool executeMarketOrders) {
  auto &stopOrders = m_orderBook->stopOrders;
  // Flag to check if any stop orders have been triggered
  bool newMarketOrders = false;

  for (auto it = stopOrders.begin(); it != stopOrders.end();) {
    StopOrder &stopOrder = *it;

    bool trigger =
        (stopOrder.isBuy && stopOrder.stopPrice <= getLatestTradedPrice()) ||
        (!stopOrder.isBuy && stopOrder.stopPrice >= getLatestTradedPrice());

    if (trigger) {
      m_orderBook->addOrder(new MarketOrder(
          stopOrder.orderID, stopOrder.traderID, stopOrder.isBuy,
          stopOrder.quantity, stopOrder.timestamp));
      it = stopOrders.erase(it);
      // Atleast one stop order has been triggered
      newMarketOrders = true;
    } else {
      ++it;
    }
  }

  // Process newly created market orders
  if (newMarketOrders && executeMarketOrders) {
    processMarketOrders();
  }
}

void MatchingEngine::processLimitOrders() {
  // Loop through all the sell orders
  for (auto it = m_orderBook->sellOrders.begin();
       it != m_orderBook->sellOrders.end();) {

    auto sellPrice = it->first;

    // If there are no buy orders at this sell price, move to the next sell
    // price
    if (m_orderBook->buyOrders.find(sellPrice) ==
        m_orderBook->buyOrders.end()) {
      ++it;
      continue;
    }

    auto &sellQueue = it->second;
    auto &buyQueue = m_orderBook->buyOrders[sellPrice];

    // Calculate total available quantity on both sides
    int totalBuyQty = 0, totalSellQty = 0;
    for (const auto &order : buyQueue)
      totalBuyQty += order.quantity;
    for (const auto &order : sellQueue)
      totalSellQty += order.quantity;

    int matchedQty = std::min(totalBuyQty, totalSellQty);

    // Match buy orders orders via pro-rata allocation
    for (auto &order : buyQueue) {
      int qtyDecrement = std::round(static_cast<double>(order.quantity) *
                                    matchedQty / totalBuyQty);
      order.quantity -= qtyDecrement;
    }

    // Match sell orders orders via pro-rata allocation
    for (auto &order : sellQueue) {
      int qtyDecrement = std::round(static_cast<double>(order.quantity) *
                                    matchedQty / totalSellQty);
      order.quantity -= qtyDecrement;
    }

    // Remove fully matched orders
    buyQueue.erase(
        std::remove_if(buyQueue.begin(), buyQueue.end(),
                       [](const LimitOrder &o) { return o.quantity == 0; }),
        buyQueue.end());

    sellQueue.erase(
        std::remove_if(sellQueue.begin(), sellQueue.end(),
                       [](const LimitOrder &o) { return o.quantity == 0; }),
        sellQueue.end());

    // Remove empty price levels
    if (sellQueue.empty())
      it = m_orderBook->sellOrders.erase(it);
    else
      ++it;

    if (buyQueue.empty())
      m_orderBook->buyOrders.erase(sellPrice);

    // If a buy order has been found then update the last traded price
    setLastTradedPrice(sellPrice);
    processStopOrders();
  }
}

double MatchingEngine::getLatestTradedPrice() {
  // If the last traded price has not been set, calculate it using the midpoint
  // of the best bid and ask
  if (m_lastTradedPrice == 0.0) {
    if (m_orderBook->buyOrders.empty() || m_orderBook->sellOrders.empty()) {
      // No market price available
      return 0.0;
    }

    double bestBid = m_orderBook->buyOrders.rbegin()->first;
    double bestAsk = m_orderBook->sellOrders.begin()->first;

    return (bestBid + bestAsk) / 2.0;
  } else {
    // Else just returnt he last traded price
    return m_lastTradedPrice;
  }
}

void MatchingEngine::setLastTradedPrice(double lastTradedPrice) {
  m_lastTradedPrice = lastTradedPrice;
}

void MatchingEngine::addOrder(Order *order) {
  m_orderBook->addOrder(order);

  if (LimitOrder *limitOrder = dynamic_cast<LimitOrder *>(order)) {
    processLimitOrders();
  } else if (MarketOrder *marketOrder = dynamic_cast<MarketOrder *>(order)) {
    processMarketOrders();
  } else if (StopOrder *stopOrder = dynamic_cast<StopOrder *>(order)) {
    processStopOrders();
  }
}
