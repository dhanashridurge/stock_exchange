#include "PortfolioManager.hpp"
 
void PortfolioManager::addUser(int id, std::string name, double balance) {
    std::lock_guard<std::mutex> lock(mtx);
    users[id] = std::make_shared<User>(id, name, balance);
}
 
std::shared_ptr<User> PortfolioManager::getUser(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    return users.count(id) ? users[id] : nullptr;
}
 
bool PortfolioManager::deposit(int userId, double amt) {
    auto u = getUser(userId);
    if (!u) return false;
    u->deposit(amt);
    return true;
}
 
bool PortfolioManager::buy(int userId, std::string symbol, int qty, double price) {
    auto u = getUser(userId);
    if (!u) return false;
 
    double total = qty * price;
    if (!u->withdraw(total)) return false;
 
    u->addStock(symbol, qty, price);
    return true;
}
 
bool PortfolioManager::sell(int userId, std::string symbol, int qty, double price) {
    auto u = getUser(userId);
    if (!u) return false;
 
    if (!u->removeStock(symbol, qty)) return false;
 
    u->deposit(qty * price);
    return true;
}
 
std::vector<std::shared_ptr<User>> PortfolioManager::getAllUsers() {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::shared_ptr<User>> res;
    for (auto &p : users) res.push_back(p.second);
    return res;
}