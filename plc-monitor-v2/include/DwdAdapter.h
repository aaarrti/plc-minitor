//
// Created by Artem Sereda on 22.10.21.
//

#ifndef PLC_MONITOR_V2_DWDADAPTER_H
#define PLC_MONITOR_V2_DWDADAPTER_H

#include <string>
class DwdAdapter {

public:

  static std::string getWeather(const std::string& id, const std::string& start, const std::string& end);

};

#endif // PLC_MONITOR_V2_DWDADAPTER_H
