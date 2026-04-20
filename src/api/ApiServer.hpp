#pragma once

#include "../services/PortfolioManager.hpp"
#include "../engine/MatchingEngine.hpp"
#include "../../external/httplib.h"

#include <unordered_map>
#include <string>

class ApiServer {

private:
    std::unordered_map<std::string, double> stockPrices;

    void loadStockPrices();
    double getStockPrice(const std::string& symbol) const;

public:
    ApiServer();
    void setup(httplib::Server& svr, PortfolioManager& pm, MatchingEngine& me);
};
