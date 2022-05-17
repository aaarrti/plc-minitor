#ifndef MyController_hpp
#define MyController_hpp

#include <fstream>
#include <oatpp/web/protocol/http/outgoing/StreamingBody.hpp>
#include <sstream>
#include <utility>

#include "SimulationService.h"
#include "StatusDTO.hpp"
#include "WindSpeedDTO.h"
#include "oatpp-swagger/AsyncController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "DwdAdapter.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen


/**
 * Sample Api Controller.
 */
class SimulationController : public oatpp::web::server::api::ApiController {

protected:
    SimulationController(const std::shared_ptr<ObjectMapper> &objectMapper) :
            oatpp::web::server::api::ApiController(objectMapper) {}

public:

    /**
     *  Inject @objectMapper component here as default parameter
     *  Do not return bare Controllable* object! use shared_ptr!
     */
    static std::shared_ptr<SimulationController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                              objectMapper)) {
        return std::shared_ptr<SimulationController>(new SimulationController(objectMapper));
    }

    ENDPOINT_INFO(windSpeed) {
        info->summary = "Write single wind speed value";
        info->addConsumes < Object < WindSpeedDTO >> ("application/json");
        info->addResponse < Object < StatusDTO >> (Status::CODE_200, "application/json");
    }

    ENDPOINT_ASYNC("PUT", "/wind-speed", windSpeed) {
    ENDPOINT_ASYNC_INIT(windSpeed)

        Action act() override {
            return request->readBodyToDtoAsync<oatpp::Object<WindSpeedDTO>>(controller->getDefaultObjectMapper())
                    .callbackTo(&windSpeed::onBodyObtained);
        }

        Action onBodyObtained(const oatpp::Object<WindSpeedDTO> &dto) {
            OATPP_COMPONENT(std::shared_ptr<SimulationService>, service);
            auto result = service->simulateWindSpeed(*dto.get());
            oatpp::web::protocol::http::Status status(result.status, result.message->c_str());
            auto response = controller->createResponse(status, result.message);
            response->putHeaderIfNotExists("Access-Control-Allow-Origin", "*");
            response->putHeaderIfNotExists("Allow", "PUT, OPTIONS");
            response->putHeaderIfNotExists("Access-Control-Allow-Methods", "PUT, OPTIONS");
            response->putHeaderIfNotExists("Access-Control-Request-Method", "PUT");
            response->putHeaderIfNotExists("Access-Control-Allow-Headers", "*");
            response->putHeaderIfNotExists("Access-Control-Request-Headers","*");
            return _return(response);
        }
    };


    ENDPOINT_INFO(windSpeedBatch) {
        info->summary = "Write batch of wind speed values";
        info->addConsumes < Object < WindSpeedBatchDTO >> ("application/json");
        info->addResponse < Object < StatusDTO >> (Status::CODE_200, "application/json");
    }

    ENDPOINT_ASYNC("PUT", "/wind-speed-batch", windSpeedBatch) {
    ENDPOINT_ASYNC_INIT(windSpeedBatch)

        Action act() override {
            return request->readBodyToDtoAsync<oatpp::Object<WindSpeedBatchDTO>>(controller->getDefaultObjectMapper())
                    .callbackTo(&windSpeedBatch::onBodyObtained);
        }

        Action onBodyObtained(const oatpp::Object<WindSpeedBatchDTO> &dto) {
            OATPP_COMPONENT(std::shared_ptr<SimulationService>, service);
            auto response = service->simulateWindSpeedBatch(*dto.get());
            oatpp::web::protocol::http::Status status(response.status, response.message->c_str());
            return _return(controller->createResponse(status, response.message));
        }
    };



    ENDPOINT_ASYNC("GET", "/ui", UI) {
    ENDPOINT_ASYNC_INIT(UI)

        Action act() override {

           auto fileStream = std::make_shared<oatpp::data::stream::FileInputStream>("../static/index.html");
           auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(fileStream);
           return _return(OutgoingResponse::createShared(Status::CODE_200, body));
        }

    };



    ENDPOINT_ASYNC("GET", "/ui/css/{filename}", GetCSSResource) {
    ENDPOINT_ASYNC_INIT(GetCSSResource)

        Action act() override {
            auto filename = request->getPathVariable("filename");
            OATPP_ASSERT_HTTP(filename, Status::CODE_400,
                              "filename should not be null")
            auto fileStream = std::make_shared<oatpp::data::stream::FileInputStream>(("../static/css/" + filename)->c_str());
            auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(fileStream);
            return _return(OutgoingResponse::createShared(Status::CODE_200, body));
        }
    };



    ENDPOINT_ASYNC("GET", "/ui/js/{filename}", GetJSResource) {
    ENDPOINT_ASYNC_INIT(GetJSResource)

        Action act() override {
            auto filename = request->getPathVariable("filename");
            auto fileStream = std::make_shared<oatpp::data::stream::FileInputStream>(("../static/js/" + filename)->c_str());
            auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(fileStream);
            return _return(OutgoingResponse::createShared(Status::CODE_200, body));
        }

    };

    ENDPOINT_ASYNC("GET", "/ui/img/{filename}", GetIMGResource) {
    ENDPOINT_ASYNC_INIT(GetIMGResource)

        Action act() override {
            auto filename = request->getPathVariable("filename");
            auto fileStream = std::make_shared<oatpp::data::stream::FileInputStream>(("../static/img/" + filename)->c_str());
            auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(fileStream);
            return _return(OutgoingResponse::createShared(Status::CODE_200, body));
        }

    };



    ENDPOINT_ASYNC("GET", "/getStations", stations) {
    ENDPOINT_ASYNC_INIT(stations)

        Action act() override{
            auto fileStream = std::make_shared<oatpp::data::stream::FileInputStream>("../static/json/dwd_stations.json");
            auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(fileStream);
            return _return(OutgoingResponse::createShared(Status::CODE_200, body));
        }
    };



    ENDPOINT_ASYNC("GET", "/getWeather", weather) {
    ENDPOINT_ASYNC_INIT(weather)

        Action act() override {

            auto param = request->getQueryParameters().getAll();
            for (auto const& [key, val] : param){
              spdlog::info("Received parameter {} = {}", key.std_str(), val.std_str());
            }

             auto &id = param["station_id"];
             auto &date_to = param["dateto"];
             auto &date_from  = param["datefrom"];
             auto res = DwdAdapter::getWeather(id.std_str(), date_to.std_str(),
                                               date_from.std_str());
             return _return(controller->createResponse(Status::CODE_200, res.c_str()));
        }

    };

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen


void runServer();

#endif /* MyController_hpp */
