#include <gtest/gtest.h>
#include "../src/valuation/PortfolioValuation.hpp"
#include "../external/json.hpp"

using json = nlohmann::json;

TEST(PortfolioValuationTest, SingleUserSingleStock) {
    json market_data = {
        {"AAPL", {{"last_price", 155.0}}}
    };
    
    json users = json::array();
    users.push_back({
        {"id", 1},
        {"name", "John Doe"},
        {"balance", 10000.0},
        {"portfolio", {
            {"AAPL", {{"quantity", 10}, {"avg_price", 150.0}}}
        }}
    });
    
    json results = valuation::value_users(market_data, users);
    
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0]["id"], 1);
    EXPECT_EQ(results[0]["name"], "John Doe");
    EXPECT_DOUBLE_EQ(results[0]["cash"], 10000.0);
    EXPECT_DOUBLE_EQ(results[0]["nav"], 11550.0); // 10000 + (10 * 155)
    
    EXPECT_EQ(results[0]["portfolio"]["AAPL"]["quantity"], 10);
    EXPECT_DOUBLE_EQ(results[0]["portfolio"]["AAPL"]["avg_price"], 150.0);
    EXPECT_DOUBLE_EQ(results[0]["portfolio"]["AAPL"]["last_price"], 155.0);
    EXPECT_DOUBLE_EQ(results[0]["portfolio"]["AAPL"]["value"], 1550.0);
}

TEST(PortfolioValuationTest, MultipleUsersMultipleStocks) {
    json market_data = {
        {"AAPL", {{"last_price", 155.0}}},
        {"GOOG", {{"last_price", 2800.0}}},
        {"MSFT", {{"last_price", 310.0}}}
    };
    
    json users = json::array();
    users.push_back({
        {"id", 1},
        {"name", "John Doe"},
        {"balance", 5000.0},
        {"portfolio", {
            {"AAPL", {{"quantity", 10}, {"avg_price", 150.0}}},
            {"GOOG", {{"quantity", 5}, {"avg_price", 2700.0}}}
        }}
    });
    users.push_back({
        {"id", 2},
        {"name", "Jane Doe"},
        {"balance", 8000.0},
        {"portfolio", {
            {"MSFT", {{"quantity", 20}, {"avg_price", 300.0}}}
        }}
    });
    
    json results = valuation::value_users(market_data, users);
    
    ASSERT_EQ(results.size(), 2);
    
    // First user
    EXPECT_EQ(results[0]["id"], 1);
    EXPECT_DOUBLE_EQ(results[0]["nav"], 20550.0); // 5000 + 1550 + 14000
    
    // Second user
    EXPECT_EQ(results[1]["id"], 2);
    EXPECT_DOUBLE_EQ(results[1]["nav"], 14200.0); // 8000 + (20 * 310)
}

TEST(PortfolioValuationTest, EmptyPortfolio) {
    json market_data = {
        {"AAPL", {{"last_price", 155.0}}}
    };
    
    json users = json::array();
    users.push_back({
        {"id", 1},
        {"name", "John Doe"},
        {"balance", 10000.0},
        {"portfolio", json::object()}
    });
    
    json results = valuation::value_users(market_data, users);
    
    ASSERT_EQ(results.size(), 1);
    EXPECT_DOUBLE_EQ(results[0]["cash"], 10000.0);
    EXPECT_DOUBLE_EQ(results[0]["nav"], 10000.0);
    EXPECT_TRUE(results[0]["portfolio"].empty());
}

TEST(PortfolioValuationTest, ProfitLossCalculation) {
    json market_data = {
        {"AAPL", {{"last_price", 160.0}}}
    };
    
    json users = json::array();
    users.push_back({
        {"id", 1},
        {"name", "John Doe"},
        {"balance", 10000.0},
        {"portfolio", {
            {"AAPL", {{"quantity", 10}, {"avg_price", 150.0}}}
        }}
    });
    
    json results = valuation::value_users(market_data, users);
    
    // P/L: (160 - 150) * 10 = +100
    // P/L %: (100 / 1500) * 100 = +6.67%
    EXPECT_EQ(results[0]["portfolio"]["AAPL"]["p_l"], "+100.00");
    EXPECT_EQ(results[0]["portfolio"]["AAPL"]["p_l_pct"], "+6.67%");
    
    // Net P/L
    EXPECT_EQ(results[0]["net"]["p_l"], "+100.00");
    EXPECT_EQ(results[0]["net"]["p_l_pct"], "+6.67%");
}

TEST(PortfolioValuationTest, LossCalculation) {
    json market_data = {
        {"AAPL", {{"last_price", 140.0}}}
    };
    
    json users = json::array();
    users.push_back({
        {"id", 1},
        {"name", "John Doe"},
        {"balance", 10000.0},
        {"portfolio", {
            {"AAPL", {{"quantity", 10}, {"avg_price", 150.0}}}
        }}
    });
    
    json results = valuation::value_users(market_data, users);
    
    // P/L: (140 - 150) * 10 = -100
    EXPECT_EQ(results[0]["portfolio"]["AAPL"]["p_l"], "-100.00");
    EXPECT_EQ(results[0]["net"]["p_l"], "-100.00");
}

TEST(PortfolioValuationTest, ZeroAvgPrice) {
    json market_data = {
        {"AAPL", {{"last_price", 150.0}}}
    };
    
    json users = json::array();
    users.push_back({
        {"id", 1},
        {"name", "John Doe"},
        {"balance", 10000.0},
        {"portfolio", {
            {"AAPL", {{"quantity", 0}, {"avg_price", 0.0}}}
        }}
    });
    
    json results = valuation::value_users(market_data, users);
    
    // Zero quantity should result in zero P/L percentage
    EXPECT_EQ(results[0]["portfolio"]["AAPL"]["p_l_pct"], "+0.00%");
    EXPECT_EQ(results[0]["net"]["p_l_pct"], "+0.00%");
}