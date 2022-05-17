#include "SimulationController.hpp"
#include "AppComponent.hpp"
#include <boost/stacktrace.hpp>
#include <iostream>
#include <boost/algorithm/string.hpp>

// Don't remove, used for weak linked consts in other .o files
const char *metricNames[] = {"Application.PLC_PRG.rProducedEnergy", "Application.PLC_PRG.rWindSpeed"};

void set_up_logger();
void sleep_on_start();

int main() {
    try {
        // General config
        assert(sizeof(float) == 4);
        set_up_logger();
        sleep_on_start();

        oatpp::base::Environment::init();
        AppComponent components;

        // PLC set up
        OATPP_COMPONENT(std::shared_ptr<PlcClient>, plcClient);
        plcClient->connect_to_plc();

        // Callback set up
        OATPP_COMPONENT(std::shared_ptr<WindSpeedChangedCallback>, callback);
        HCYCLIST cycList = plcClient->cyc_define_vars(callback.get());
        callback->setList(cycList);

        // REST endpoint for simulation set up
        runServer();
        oatpp::base::Environment::destroy();
    } catch (std::exception &e) {
        spdlog::error("Ooops, Something went wrong! :(\n {}", e.what());
        std::cerr << boost::stacktrace::stacktrace();
        return 1;
    }
    return 0;
}

void set_up_logger() {
    if (const char *env_p = std::getenv("LOG_LEVEL")) {
        auto level = spdlog::level::from_str(boost::algorithm::to_lower_copy(std::string(env_p)));
        spdlog::set_level(level);
    } else {
        spdlog::set_level(spdlog::level::level_enum::trace);
        spdlog::debug("LOG_LEVEL not set, default to TRACE");
    }
}

void sleep_on_start() {
    int sleep = 0;
    if (const char *env_p = std::getenv("STARTUP_SLEEP_S")) {
        sleep = std::stoi(env_p);
    }
    spdlog::debug("Waiting for {}s for windturbine to start up", sleep);
    std::this_thread::sleep_for(std::chrono::seconds(sleep));
}



