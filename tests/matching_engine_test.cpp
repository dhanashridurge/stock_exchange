#include <gtest/gtest.h>
#include <thread>
#include "../src/engine/MatchingEngine.hpp"
#include "../src/services/PortfolioManager.hpp"
 
TEST(MatchingEngineTest, OrderTypeValues) {
    Order buyOrder{1, "GOOG", 5, 2800.0, OrderType::BUY};
    Order sellOrder{2, "GOOG", 5, 2800.0, OrderType::SELL};
    
    EXPECT_EQ(buyOrder.type, OrderType::BUY);
    EXPECT_EQ(sellOrder.type, OrderType::SELL);
}
 
TEST(MatchingEngineTest, OrderFields) {
    Order order{1, "AAPL", 10, 150.0, OrderType::BUY};
    EXPECT_EQ(order.userId, 1);
    EXPECT_EQ(order.symbol, "AAPL");
    EXPECT_EQ(order.qty, 10);
    EXPECT_DOUBLE_EQ(order.price, 150.0);
}
 
TEST(MatchingEngineTest, DifferentSymbols) {
    Order aapl{1, "AAPL", 10, 150.0, OrderType::BUY};
    Order msft{2, "MSFT", 20, 300.0, OrderType::BUY};
    Order tsla{3, "TSLA", 5, 200.0, OrderType::BUY};
    
    EXPECT_EQ(aapl.symbol, "AAPL");
    EXPECT_EQ(msft.symbol, "MSFT");
    EXPECT_EQ(tsla.symbol, "TSLA");
}