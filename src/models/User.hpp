#pragma once

#include <string>

#include <unordered_map>

#include <mutex>
 
class User {

private:

    int id;

    std::string name;

    double balance;

    std::unordered_map<std::string, int> portfolio;

    std::mutex mtx;
 
public:

    User(int id, std::string name, double balance);
 
    int getId();

    std::string getName();
 
    double getBalance();

    void deposit(double amt);

    bool withdraw(double amt);
 
    void addStock(std::string symbol, int qty);

    bool removeStock(std::string symbol, int qty);
 
    std::unordered_map<std::string, int> getPortfolio();

};
 