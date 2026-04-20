#pragma once

#include "../models/User.hpp"

#include <unordered_map>

#include <memory>

#include <mutex>

#include <vector>
 
class PortfolioManager {

private:

    std::unordered_map<int, std::shared_ptr<User>> users;

    std::mutex mtx;
 
public:

    void addUser(int id, std::string name, double balance);

    std::shared_ptr<User> getUser(int id);
 
    bool deposit(int userId, double amt);

    bool buy(int userId, std::string symbol, int qty, double price);

    bool sell(int userId, std::string symbol, int qty, double price);
 
    std::vector<std::shared_ptr<User>> getAllUsers();

};
 