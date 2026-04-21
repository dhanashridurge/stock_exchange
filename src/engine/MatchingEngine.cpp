#include "MatchingEngine.hpp"

#include <iostream>
 
MatchingEngine::MatchingEngine(PortfolioManager& pm) : pm(pm), running(true) {}
 
void MatchingEngine::start() {
    std::thread([this]() { process(); }).detach();
}
 
void MatchingEngine::submit(Order o) {
    queue.push(o);
}
 
void MatchingEngine::process() {
    while (running) {
        Order o = queue.pop();
        if (o.type == OrderType::BUY) {
            for (auto it = sellOrders.begin(); it != sellOrders.end(); ++it) {
                if (it->symbol == o.symbol && it->price <= o.price) {
                    int qty = std::min(o.qty, it->qty);
                    pm.buy(o.userId, o.symbol, qty, it->price);
                    pm.sell(it->userId, it->symbol, qty, it->price);
                    it->qty -= qty;
                    o.qty -= qty;
                    if (it->qty == 0) sellOrders.erase(it);

                    break;
                }
            }

            if (o.qty > 0) buyOrders.push_back(o);

        } else {
            sellOrders.push_back(o);
        }
    }
}
 