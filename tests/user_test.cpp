#include <gtest/gtest.h>
#include "../src/models/User.hpp"
 
TEST(UserTest, CreateUser) {
    User user(1, "John Doe", 10000.0);
    EXPECT_EQ(user.getId(), 1);
    EXPECT_EQ(user.getName(), "John Doe");
    EXPECT_DOUBLE_EQ(user.getBalance(), 10000.0);
}
 
TEST(UserTest, Deposit) {
    User user(2, "Jane Doe", 5000.0);
    user.deposit(1000.0);
    EXPECT_DOUBLE_EQ(user.getBalance(), 6000.0);
}
 
TEST(UserTest, WithdrawSuccess) {
    User user(3, "Bob Smith", 5000.0);
    bool result = user.withdraw(2000.0);
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(user.getBalance(), 3000.0);
}
 
TEST(UserTest, WithdrawInsufficientFunds) {
    User user(4, "Alice Smith", 1000.0);
    bool result = user.withdraw(2000.0);
    EXPECT_FALSE(result);
    EXPECT_DOUBLE_EQ(user.getBalance(), 1000.0);
}
 
TEST(UserTest, AddStock) {
    User user(5, "Charlie", 10000.0);
    user.addStock("AAPL", 10, 150.0);
    user.addStock("AAPL", 10, 160.0);
    
    auto it = user.getPortfolio().find("AAPL");
    ASSERT_NE(it, user.getPortfolio().end());
    EXPECT_EQ(it->second.quantity, 20);
    EXPECT_DOUBLE_EQ(it->second.avgPrice, 155.0);
}
 
TEST(UserTest, RemoveStockSuccess) {
    User user(6, "David", 10000.0);
    user.addStock("GOOG", 100, 2800.0);
    bool result = user.removeStock("GOOG", 50);
    EXPECT_TRUE(result);
    
    auto it = user.getPortfolio().find("GOOG");
    ASSERT_NE(it, user.getPortfolio().end());
    EXPECT_EQ(it->second.quantity, 50);
}
 
TEST(UserTest, RemoveStockInsufficient) {
    User user(7, "Eve", 10000.0);
    user.addStock("MSFT", 10, 300.0);
    bool result = user.removeStock("MSFT", 20);
    EXPECT_FALSE(result);
}
 
TEST(UserTest, RemoveStockNotOwned) {
    User user(8, "Frank", 10000.0);
    bool result = user.removeStock("TSLA", 10);
    EXPECT_FALSE(result);
}