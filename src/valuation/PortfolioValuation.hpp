#pragma once

#include "../external/json.hpp"

namespace valuation {

using json = nlohmann::json;

/**
 * @brief Compute valuation for multiple users
 *
 * @param market_data  JSON object keyed by symbol with at least "last_price"
 * @param users        JSON array of user objects
 * @return             JSON array with valuation results per user
 */
json value_users(const json &market_data, const json &users);

} // namespace valuation