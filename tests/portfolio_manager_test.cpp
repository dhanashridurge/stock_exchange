#include <gtest/gtest.h>
#include "../src/services/PortfolioManager.hpp"
#include "../src/models/User.hpp"

TEST(PortfolioManagerTest, AddUser) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    
    auto user = pm.getUser(1);
    ASSERT_NE(user, nullptr);
    EXPECT_EQ(user->getId(), 1);
    EXPECT_EQ(user->getName(), "John Doe");
    EXPECT_DOUBLE_EQ(user->getBalance(), 10000.0);
}

TEST(PortfolioManagerTest, GetUserNotFound) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    
    auto user = pm.getUser(999);
    EXPECT_EQ(user, nullptr);
}

TEST(PortfolioManagerTest, Deposit) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    
    bool result = pm.deposit(1, 5000.0);
    EXPECT_TRUE(result);
    
    auto user = pm.getUser(1);
    EXPECT_DOUBLE_EQ(user->getBalance(), 15000.0);
}

TEST(PortfolioManagerTest, DepositInvalidUser) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    
    bool result = pm.deposit(999, 5000.0);
    EXPECT_FALSE(result);
}

TEST(PortfolioManagerTest, BuyStock) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    
    bool result = pm.buy(1, "AAPL", 10, 150.0);
    EXPECT_TRUE(result);
    
    auto user = pm.getUser(1);
    EXPECT_DOUBLE_EQ(user->getBalance(), 8500.0); // 10000 - (10 * 150)
    
    auto it = user->getPortfolio().find("AAPL");
    ASSERT_NE(it, user->getPortfolio().end());
    EXPECT_EQ(it->second.quantity, 10);
    EXPECT_DOUBLE_EQ(it->second.avgPrice, 150.0);
}

TEST(PortfolioManagerTest, BuyStockInsufficientFunds) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 1000.0);
    
    bool result = pm.buy(1, "AAPL", 10, 150.0);
    EXPECT_FALSE(result); // Should fail - need 1500, have 1000
    
    auto user = pm.getUser(1);
    EXPECT_DOUBLE_EQ(user->getBalance(), 1000.0); // Unchanged
}

TEST(PortfolioManagerTest, BuyStockInvalidUser) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    
    bool result = pm.buy(999, "AAPL", 10, 150.0);
    EXPECT_FALSE(result);
}

TEST(PortfolioManagerTest, SellStock) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    pm.buy(1, "AAPL", 10, 150.0); // Buy at 150
    
    bool result = pm.sell(1, "AAPL", 5, 160.0); // Sell at 160
    EXPECT_TRUE(result);
    
    auto user = pm.getUser(1);
    EXPECT_DOUBLE_EQ(user->getBalance(), 9300.0); // 8500 + (5 * 160)
    
    auto it = user->getPortfolio().find("AAPL");
    ASSERT_NE(it, user->getPortfolio().end());
    EXPECT_EQ(it->second.quantity, 5);
}

TEST(PortfolioManagerTest, SellStockInsufficientQuantity) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    pm.buy(1, "AAPL", 10, 150.0);
    
    bool result = pm.sell(1, "AAPL", 20, 160.0);
    EXPECT_FALSE(result); // Can't sell 20, only own 10
    
    auto user = pm.getUser(1);
    auto it = user->getPortfolio().find("AAPL");
    ASSERT_NE(it, user->getPortfolio().end());
    EXPECT_EQ(it->second.quantity, 10); // Unchanged
}

TEST(PortfolioManagerTest, SellStockNotOwned) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    
    bool result = pm.sell(1, "TSLA", 10, 200.0);
    EXPECT_FALSE(result);
}

TEST(PortfolioManagerTest, GetAllUsers) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    pm.addUser(2, "Jane Doe", 5000.0);
    pm.addUser(3, "Bob Smith", 7500.0);
    
    auto users = pm.getAllUsers();
    EXPECT_EQ(users.size(), 3);
}

TEST(PortfolioManagerTest, MultipleBuysSameStock) {
    PortfolioManager pm;
    pm.addUser(1, "John Doe", 10000.0);
    
    pm.buy(1, "AAPL", 10, 150.0);
    pm.buy(1, "AAPL", 10, 160.0);
    
    auto user = pm.getUser(1);
    auto it = user->getPortfolio().find("AAPL");
    ASSERT_NE(it, user->getPortfolio().end());
    EXPECT_EQ(it->second.quantity, 20);
    EXPECT_DOUBLE_EQ(it->second.avgPrice, 155.0); // (10*150 + 10*160) / 20
}