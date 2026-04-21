#pragma once

#include "../core/ThreadSafeQueue.hpp"
#include "../models/Order.hpp"
#include "../services/PortfolioManager.hpp"
#include <thread>
#include <vector>
class MatchingEngine {

private:

    ThreadSafeQueue<Order> queue;
    std::vector<Order> buyOrders;
    std::vector<Order> sellOrders;
    PortfolioManager& pm;
    bool running;
    void process();
 
public:

    MatchingEngine(PortfolioManager& pm);
    void start();
    void submit(Order o);
};
 