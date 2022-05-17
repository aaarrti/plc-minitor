//
// Created by Artem Sereda on 22.07.21.
//

#include "SimulationService.h"
#include "AppComponent.hpp"

SimulationService::SimulationService() {
    OATPP_COMPONENT(std::shared_ptr<PlcClient>, plcClient);
    this->plcClient = plcClient;
    this->OK_RESPONSE.status = 200;
    this->OK_RESPONSE.message = "OK";
}

StatusDTO SimulationService::simulateWindSpeed(const WindSpeedDTO& windSpeedDto) {
    this->plcClient->sync_write_windspeed(windSpeedDto.windSpeed);
    return this->OK_RESPONSE;
}

StatusDTO SimulationService::simulateWindSpeedBatch(const WindSpeedBatchDTO& windSpeedBatchDto) {
    for (const auto &i: *windSpeedBatchDto.windSpeed) {
        this->plcClient->sync_write_windspeed(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(windSpeedBatchDto.updateRateMs));
    }
    return this->OK_RESPONSE;
}
