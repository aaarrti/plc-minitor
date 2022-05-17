//
// Created by Artem Sereda on 13.07.21.
//

#include "PrometheusClient.hpp"
#include "AppComponent.hpp"
#include <prometheus/counter.h>
#include <prometheus/exposer.h>
#include <prometheus/registry.h>


using namespace prometheus;
using namespace std;


PrometheusClient::PrometheusClient() {
    OATPP_COMPONENT(std::shared_ptr<libconfig::Config>, config);
    int port = config->lookup("exposerPort");
    this->exposer = make_shared<Exposer>("0.0.0.0:" + to_string(port));
    this->registry = std::make_shared<Registry>();
    auto& wG = BuildGauge().Name("wind_speed").Help("Wind speed in m/s").Register(*registry).Add({{"X", "time"}});
    this->windSpeedGauge = shared_ptr<Gauge>(&wG);
    this->windSpeedGauge->Set(0);
    auto& peG = BuildGauge().Name("produced_energy").Help("Produced Energy in MW").Register(*registry).Add({{"X","time"}});
    this->producedEnergyGauge = shared_ptr<Gauge>(&peG);
    this->producedEnergyGauge->Set(0);
    exposer->RegisterCollectable(registry);
}

void PrometheusClient::push_metric(float windSpeed, float producedEnergy) {
    spdlog::debug("Pushing metrics to prometheus windSpeed = {}, ProducedEnergy = {}", windSpeed, producedEnergy);
    this->windSpeedGauge->Set(windSpeed);
    this->producedEnergyGauge->Set(producedEnergy);
}

void PrometheusClient::push_wind_speed_metric(float windSpeed) {
    spdlog::debug("Pushing metrics to prometheus windSpeed = {}", windSpeed);
    this->windSpeedGauge->Set(windSpeed);
}
