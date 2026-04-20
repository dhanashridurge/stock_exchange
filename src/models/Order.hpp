#pragma once
#include <string>
 
enum class OrderType { BUY, SELL };
 
struct Order {
    int userId;
    std::string symbol;
    int qty;
    double price;
    OrderType type;
};