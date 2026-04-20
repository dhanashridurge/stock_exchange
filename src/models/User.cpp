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
 
void User::addStock(std::string symbol, int qty) {

    std::lock_guard<std::mutex> lock(mtx);

    portfolio[symbol] += qty;

}
 
bool User::removeStock(std::string symbol, int qty) {

    std::lock_guard<std::mutex> lock(mtx);

    if (portfolio[symbol] >= qty) {

        portfolio[symbol] -= qty;

        return true;

    }

    return false;

}
 
std::unordered_map<std::string, int> User::getPortfolio() {

    std::lock_guard<std::mutex> lock(mtx);

    return portfolio;

}
 