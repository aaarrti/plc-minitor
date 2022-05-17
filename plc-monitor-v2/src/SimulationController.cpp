//
// Created by Artem Sereda on 13.07.21.
//

#include <AppComponent.hpp>
#include <oatpp/network/Server.hpp>
#include "SimulationController.hpp"

void runServer() {

    /* Get router component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    /* Create SimulationController and add all of its endpoints to router */
    auto myController = SimulationController::createShared();
    myController->addEndpointsToRouter(router);

    /* Get connection handler component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    /* Get connection provider component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
    oatpp::network::Server server(connectionProvider, connectionHandler);

    /* Priny info about server port */
    spdlog::info("swagger UI available on http://localhost:8081/swagger/ui/");
    spdlog::info("simulation UI available on http://localhost:8081/ui/");

    auto docEndpoints = oatpp::swagger::AsyncController::Endpoints::createShared();
    docEndpoints->pushBackAll(myController->getEndpoints());
    auto swaggerController = oatpp::swagger::AsyncController::createShared(docEndpoints);
    swaggerController->addEndpointsToRouter(router);

    router->logRouterMappings();
    /* Run server */
    server.run();

    /* Print how much objects were created during app running, and what have left-probably leaked */
    /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
    spdlog::info("\nEnvironment:\n objectsCount = {}\n objectsCreated = {}",
                 oatpp::base::Environment::getObjectsCount(), oatpp::base::Environment::getObjectsCreated());
}
