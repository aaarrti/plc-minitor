//
// Created by Artem Sereda on 13.07.21.
//

#ifndef PLC_MONITOR_V2_PROMETHEUSCLIENT_H
#define PLC_MONITOR_V2_PROMETHEUSCLIENT_H
#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include <prometheus/gauge.h>
#include <memory>


class PrometheusClient {

public:
    PrometheusClient();
    void push_metric(float windSpeed, float producedEnergy);
    void push_wind_speed_metric(float windSpeed);

private:
    std::shared_ptr<prometheus::Exposer> exposer;
    std::shared_ptr<prometheus::Registry> registry;
    std::shared_ptr<prometheus::Gauge> windSpeedGauge;
    std::shared_ptr<prometheus::Gauge> producedEnergyGauge;

};


#endif //PLC_MONITOR_V2_PROMETHEUSCLIENT_H
