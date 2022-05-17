#ifndef AppComponent_hpp
#define AppComponent_hpp

#include <PrometheusClient.hpp>
#include <WindSpeedChangedCallback.hpp>
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/component.hpp"
#include "libconfig.h++"
#include "oatpp-swagger/oas3/Model.hpp"
#include "oatpp/core/Types.hpp"
#include "RequestInterceptor.h"
#include "SimulationService.h"
#include "DwdAdapter.h"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:

    OATPP_CREATE_COMPONENT(std::shared_ptr<libconfig::Config>, _config)([]{
        auto conf = std::make_shared<libconfig::Config>();
        conf->readFile("../resources/application.cfg");
        return conf;
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
        OATPP_COMPONENT(std::shared_ptr<libconfig::Config>, config);
        int data_threads = config->lookup("data-threads");
        int io_threads = config->lookup("io-threads");
        int timer_threads = config->lookup("timer-threads");
        return std::make_shared<oatpp::async::Executor>(data_threads, io_threads, timer_threads);
    }());


    /**
     *  Create ConnectionProvider component which listens on the port
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
        OATPP_COMPONENT(std::shared_ptr<libconfig::Config>, config);
        int port = config->lookup("serverPort");
        return oatpp::network::tcp::server::ConnectionProvider::createShared(
                {"0.0.0.0", static_cast<v_uint16>(port), oatpp::network::Address::IP_4});
    }());

    /**
     *  Create Router component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
        OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, _executor);
        auto httpConnectionHandler = oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, _executor);
        auto errorHandler = std::make_shared<ExceptionLogger>();
        httpConnectionHandler->setErrorHandler(errorHandler);
        // hopefully, this will fix CORS issue
        auto optionsInterceptor = std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>();
        httpConnectionHandler->addRequestInterceptor(optionsInterceptor);
        auto corsInterceptor = std::make_shared<CORSInterceptor>();
        httpConnectionHandler->addResponseInterceptor(corsInterceptor);
        return httpConnectionHandler;
    }());


    /**
     *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
        auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
        auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
        deserializerConfig->allowUnknownFields = false;
        auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared(serializerConfig, deserializerConfig);
        return objectMapper;
    }());

    /**
     *  General API docs info
     *  */

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {
        OATPP_COMPONENT(std::shared_ptr<libconfig::Config>, config);
        int port = config->lookup("serverPort");
        auto str = "http://localhost:" + std::to_string(port);
        const char *server = str.c_str();
        oatpp::swagger::DocumentInfo::Builder builder;
        builder
                .setTitle("Wind speed change simulation c++ API")
                .setVersion("0.1")
                .addServer(server, "nuff said");
        return builder.build();
    }());

    /**
    *  Swagger-Ui Resources
     *
     *  */

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
        return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
    }());

    /*
     * Components required by application
     * */

    OATPP_CREATE_COMPONENT(std::shared_ptr<PlcClient>, plcClient)([]{
        return std::make_shared<PlcClient>();
    }());


    OATPP_CREATE_COMPONENT(std::shared_ptr<PrometheusClient>, plrometbhuesClient)([]{
        return std::make_shared<PrometheusClient>();
    }());


    OATPP_CREATE_COMPONENT(std::shared_ptr<WindSpeedChangedCallback>, callback)([]{
        return std::make_shared<WindSpeedChangedCallback>();
    }());


    OATPP_CREATE_COMPONENT(std::shared_ptr<SimulationService>, simService)([]{
        return std::make_shared<SimulationService>();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<DwdAdapter>, adapter)([]{
      return std::make_shared<DwdAdapter>();
    }());

private:

};

#endif /* AppComponent_hpp */
