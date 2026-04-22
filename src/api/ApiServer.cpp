#include "ApiServer.hpp"
#include "../external/json.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
using json = nlohmann::json;

ApiServer::ApiServer() {
    loadStockPrices();
}

void ApiServer::loadStockPrices() {
    std::ifstream file("data/stocks.json");
    if (!file.good()) {
        std::cerr << "Warning: unable to open data/stocks.json" << std::endl;
        return;
    }
    json j;
    file >> j;
    for (auto& [symbol, value] : j.items()) {
        stockPrices[symbol] = value.get<double>();
    }
}

double ApiServer::getStockPrice(const std::string& symbol) const {
    auto it = stockPrices.find(symbol);
    return it == stockPrices.end() ? 0.0 : it->second;
}

void ApiServer::setup(httplib::Server& svr, PortfolioManager& pm, MatchingEngine& me) {
    // Add CORS headers to all responses
    svr.set_post_routing_handler([](const auto& req, auto& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    });

    // Handle OPTIONS requests for CORS preflight
    svr.Options("/.*", [](const auto&, auto& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_content("", "text/plain");
    });

    svr.Get("/", [](auto&, auto& res) {
        std::ifstream file("web/index.html");
        if (file.good()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            res.set_content(buffer.str(), "text/html");
        } else {
            res.set_content("<h1>Welcome to Stock Exchange</h1>", "text/html");
        }
    });

    svr.Post("/buy", [&](auto& req, auto& res) {
        auto j = json::parse(req.body);
        bool success = pm.buy(j["userId"], j["symbol"], j["qty"], j["price"]);
        if (success) {
            res.set_content("Buy Order Executed", "text/plain");
        } else {
            res.status = 400;
            res.set_content("Buy Order Failed: insufficient funds", "text/plain");
        }
    });
 
    svr.Post("/sell", [&](auto& req, auto& res) {
        auto j = json::parse(req.body);
        bool success = pm.sell(j["userId"], j["symbol"], j["qty"], j["price"]);
        if (success) {
            res.set_content("Sell Order Executed", "text/plain");
        } else {
            res.status = 400;
            res.set_content("Sell Order Failed: insufficient holdings", "text/plain");
        }
    });
 
    svr.Get("/users", [&](auto&, auto& res) {
        auto users = pm.getAllUsers();
        json arr = json::array();
        for (auto u : users) {
            json j = json::object();
            j["id"] = u->getId();
            j["name"] = u->getName();
            j["balance"] = u->getBalance();
            json portfolio_json = json::object();
            for (const auto& [symbol, holding] : u->getPortfolio()) {
                portfolio_json[symbol] = {
                    {"quantity", holding.quantity},
                    {"avg_price", holding.avgPrice}
                };
        }
        j["portfolio"] = portfolio_json;
        // NAV calculation
        double nav = u->getBalance();
        for (const auto& [symbol, holding] : u->getPortfolio()) {
            nav += holding.quantity * getStockPrice(symbol);
        }
        j["nav"] = nav;
        arr.push_back(j);
        }
        res.set_content(arr.dump(4), "application/json");
    });
}
 