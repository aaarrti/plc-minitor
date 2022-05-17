//
// Created by Artem Sereda on 29.06.21.
//

#include <future>
#include "WindSpeedChangedCallback.hpp"
#include "AppComponent.hpp"

using namespace std::chrono;

extern char *metricNames[];

long WindSpeedChangedCallback::Notify(CPLCHandler *pPlcHandler, CallbackAddInfoTag callbackAdditionalInfo) {
    std::async(std::launch::async | std::launch::deferred, [this, pPlcHandler]() {
        spdlog::trace("Notify called");
        PlcVarValue **pPlcVarValue;
        unsigned long numOfChangedValues = 0;
        unsigned long *ppChangedValuesIndex;
        pPlcHandler->CycReadChangedVars(this->list, &pPlcVarValue, &ppChangedValuesIndex, &numOfChangedValues);
        if (numOfChangedValues == 2) {
            auto producedEnergy = *(float *) pPlcVarValue[0]->byData;
            auto windSpeed = *(float *) pPlcVarValue[1]->byData;
            prometheusClient->push_metric(windSpeed, producedEnergy);
            return;
        }
        if (numOfChangedValues == 1){
            auto windSpeed = *(float *) pPlcVarValue[0]->byData;
            prometheusClient->push_wind_speed_metric(windSpeed);
            return;
        }
        spdlog::warn("Unexpected number of values changed {}", numOfChangedValues);
    }).share();
    return 0;
}


void WindSpeedChangedCallback::setList(HCYCLIST hcyclist) {
    this->list = hcyclist;
}

WindSpeedChangedCallback::WindSpeedChangedCallback() {
    OATPP_COMPONENT(std::shared_ptr<PrometheusClient>, prometheusClient);
    this->prometheusClient = prometheusClient;
}
