#include <gtest/gtest.h>
#include "../src/models/Order.hpp"
 
TEST(OrderTest, CreateOrder) {
    Order order{1, "AAPL", 100, 150.50, OrderType::BUY};
    EXPECT_EQ(order.userId, 1);
    EXPECT_EQ(order.symbol, "AAPL");
    EXPECT_EQ(order.qty, 100);
    EXPECT_DOUBLE_EQ(order.price, 150.50);
    EXPECT_EQ(order.type, OrderType::BUY);
}
 
TEST(OrderTest, OrderTypeValues) {
    Order buyOrder{1, "GOOG", 50, 2800.0, OrderType::BUY};
    Order sellOrder{2, "MSFT", 25, 300.0, OrderType::SELL};
    
    EXPECT_EQ(buyOrder.type, OrderType::BUY);
    EXPECT_EQ(sellOrder.type, OrderType::SELL);
}
 
TEST(OrderTest, ModifyOrderQuantity) {
    Order order{1, "TSLA", 10, 200.0, OrderType::BUY};
    order.qty = 20;
    EXPECT_EQ(order.qty, 20);
}