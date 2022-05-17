//
// Created by Artem Sereda on 01.08.21.
//

#ifndef PLC_MONITOR_V2_REQUESTINTERCEPTOR_H
#define PLC_MONITOR_V2_REQUESTINTERCEPTOR_H

#include "oatpp/web/server/interceptor/RequestInterceptor.hpp"
#include "spdlog/spdlog.h"
#include "oatpp/web/server/interceptor/AllowCorsGlobal.hpp"


class ExceptionLogger : public oatpp::web::server::handler::DefaultErrorHandler {

public:
    std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> handleError(const oatpp::web::protocol::http::Status &status,
                                                                                const oatpp::String &message, const Headers &headers) override {
        spdlog::error(message->std_str());
        return DefaultErrorHandler::handleError(status, message, headers);
    }

};


class CORSInterceptor : public oatpp::web::server::interceptor::AllowCorsGlobal {

public:
    std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest> &request,
                                                const std::shared_ptr<OutgoingResponse> &response) override {
        auto result = AllowCorsGlobal::intercept(request, response);
        result->putHeader(oatpp::web::protocol::http::Header::CORS_METHODS, this->CORS_METHODS);
        return result;
    }

private:
    oatpp::String CORS_METHODS = "PUT, OPTIONS";

};


#endif //PLC_MONITOR_V2_REQUESTINTERCEPTOR_H
