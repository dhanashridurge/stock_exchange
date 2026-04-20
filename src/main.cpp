#include "api/ApiServer.hpp"
#include "services/PortfolioManager.hpp"
#include "engine/MatchingEngine.hpp"
#include "../external/httplib.h"
 
int main() {
    httplib::Server svr;
 
    PortfolioManager pm;
    pm.addUser(1, "Alice", 10000);
    pm.addUser(2, "Bob", 8000);
 
    MatchingEngine me(pm);
    me.start();
 
    ApiServer api;
    api.setup(svr, pm, me);
 
    svr.listen("0.0.0.0", 8080);
}