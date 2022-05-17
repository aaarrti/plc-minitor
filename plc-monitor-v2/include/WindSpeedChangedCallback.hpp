//
// Created by Artem Sereda on 29.06.21.
//

#ifndef PLC_READER_WINDSPEEDCHANGEDCALLBACK_HPP
#define PLC_READER_WINDSPEEDCHANGEDCALLBACK_HPP
#pragma once
#include "PLCHandler.h"
#include "PlcClient.hpp"
#include "spdlog/spdlog.h"
#include <utility>
#include "libconfig.h++"
#include "PrometheusClient.hpp"

class WindSpeedChangedCallback : public CPLCHandlerCallback {

private:
    HCYCLIST list;
    std::shared_ptr<PrometheusClient> prometheusClient;

public:
    explicit WindSpeedChangedCallback();
    long Notify(CPLCHandler *pPlcHandler, CallbackAddInfoTag CallbackAdditionalInfo) override;
    void setList(HCYCLIST hcyclist);
};
#endif //PLC_READER_WINDSPEEDCHANGEDCALLBACK_HPP
