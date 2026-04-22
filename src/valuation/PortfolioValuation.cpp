#include "PortfolioValuation.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

namespace valuation {

static std::string signed_fmt(double value, bool pct = false) {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(2);
  if (value >= 0.0)
    oss << "+";
  oss << value;
  if (pct)
    oss << "%";
  return oss.str();
}

json value_users(const json &market_data, const json &users) {
  json results = json::array();

  for (const auto &user : users) {
    double invested_total = 0.0;
    double current_total = 0.0;

    json per_stock = json::object();

    // portfolio is now an object keyed by symbol
    for (const auto &[symbol, pos] : user.at("portfolio").items()) {

      const int qty = pos.at("quantity");
      const double avg_price = pos.at("avg_price");
      const double last_price = market_data.at(symbol).at("last_price");

      const double invested = qty * avg_price;
      const double current = qty * last_price;
      const double pl = current - invested;
      const double pl_pct = invested > 0.0 ? (pl / invested) * 100.0 : 0.0;

      invested_total += invested;
      current_total += current;

      per_stock[symbol] = {{"quantity", qty},
                           {"avg_price", avg_price},
                           {"last_price", last_price},
                           {"value", std::round(current * 100.0) / 100.0},
                           {"p_l", signed_fmt(pl)},
                           {"p_l_pct", signed_fmt(pl_pct, true)}};
    }

    const double cash = user.at("balance");
    const double nav = cash + current_total;

    const double net_pl = current_total - invested_total;
    const double net_pl_pct =
        invested_total > 0.0 ? (net_pl / invested_total) * 100.0 : 0.0;

    json user_result = {{"id", user.at("id")},
                        {"name", user.at("name")},
                        {"cash", cash},
                        {"nav", std::round(nav * 100.0) / 100.0},
                        {"portfolio", per_stock},
                        {"net",
                         {{"p_l", signed_fmt(net_pl)},
                          {"p_l_pct", signed_fmt(net_pl_pct, true)}}}};

    results.push_back(user_result);
  }

  return results;
}

} // namespace valuation