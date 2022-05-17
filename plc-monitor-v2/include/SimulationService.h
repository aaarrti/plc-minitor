//
// Created by Artem Sereda on 22.07.21.
//

#ifndef PLC_MONITOR_V2_SIMULATIONSERVICE_H
#define PLC_MONITOR_V2_SIMULATIONSERVICE_H

#include <StatusDTO.hpp>
#include "WindSpeedDTO.h"
#include "memory.h"
#include "PlcClient.hpp"


class SimulationService {

public:
    SimulationService();
    StatusDTO simulateWindSpeed(const WindSpeedDTO& windSpeedDto);

    StatusDTO simulateWindSpeedBatch(const WindSpeedBatchDTO& windSpeedBatchDto);

private:
    std::shared_ptr<PlcClient> plcClient;
    StatusDTO OK_RESPONSE;

};


#endif //PLC_MONITOR_V2_SIMULATIONSERVICE_H
