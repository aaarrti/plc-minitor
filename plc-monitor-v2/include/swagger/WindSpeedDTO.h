//
// Created by Artem Sereda on 22.07.21.
//

#ifndef PLC_MONITOR_V2_WINDSPEEDDTO_H
#define PLC_MONITOR_V2_WINDSPEEDDTO_H

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)//"oatpp/codegen/DTO_define.hpp"


class WindSpeedDTO : public oatpp::DTO {

    DTO_INIT(WindSpeedDTO, DTO)
    DTO_FIELD(Float32 , windSpeed);


};

class WindSpeedBatchDTO : public oatpp::DTO {

    DTO_INIT(WindSpeedBatchDTO, DTO)
    DTO_FIELD(List<Float32> , windSpeed);
    DTO_FIELD(UInt32, updateRateMs);

};


#endif //PLC_MONITOR_V2_WINDSPEEDDTO_H
