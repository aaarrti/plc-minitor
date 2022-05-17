//
// Created by Artem Sereda on 20.06.21.
//

#ifndef PLC_READER_PLCCLIENT_HPP
#define PLC_READER_PLCCLIENT_HPP
#pragma once
#include "PLCHandler.h"
#include "string"
#include <utility>
#include <thread>
#include <oatpp/core/data/mapping/type/Primitive.hpp>
#include "spdlog/spdlog.h"
#include "SysTaskItf.h"
#include "libconfig.h++"
#include "chrono"

class PlcClient {

private:
    CEasyPLCHandler *plcHandler;
    std::shared_ptr<libconfig::Config> config;
    static std::string decode_error_code(long code);

public:
    explicit PlcClient();
    void connect_to_plc();
    void sync_write_windspeed(float newValue);
    HCYCLIST cyc_define_vars(CPLCHandlerCallback* callback);
};


#endif //PLC_READER_PLCCLIENT_HPP
