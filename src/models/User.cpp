#include "User.hpp"
 
User::User(int id, std::string name, double balance)
    : id(id), name(name), balance(balance) {}
 
int User::getId() { return id; }

std::string User::getName() { return name; }
 
double User::getBalance() {
    std::lock_guard<std::mutex> lock(mtx);
    return balance;
}
 
void User::deposit(double amt) {
    std::lock_guard<std::mutex> lock(mtx);
    balance += amt;
}
 
bool User::withdraw(double amt) {
    std::lock_guard<std::mutex> lock(mtx);
    if (balance >= amt) {
        balance -= amt;
        return true;
    }
    return false;
}
 
void User::addStock(std::string symbol, int qty, double price) {
   std::lock_guard<std::mutex> lock(mtx);
   auto& holding = portfolio[symbol];
   int oldQty = holding.quantity;
   double oldAvg = holding.avgPrice;
   int newQty = oldQty + qty;
   double newAvg = 0.0;
   if (newQty > 0) {
       newAvg = ((oldQty * oldAvg) + (qty * price)) / newQty;
   }
   holding.quantity = newQty;
   holding.avgPrice = newAvg;
}
 
bool User::removeStock(std::string symbol, int qty) {
   std::lock_guard<std::mutex> lock(mtx);
   auto it = portfolio.find(symbol);
   if (it == portfolio.end() || it->second.quantity < qty)
       return false;
   it->second.quantity -= qty;
   if (it->second.quantity == 0)
       portfolio.erase(it);
   return true;
}
 
std::unordered_map<std::string, StockHolding> User::getPortfolio() {
    std::lock_guard<std::mutex> lock(mtx);
    return portfolio;
}
 