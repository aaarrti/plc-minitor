#ifndef CRUD_STATUSDTO_HPP
#define CRUD_STATUSDTO_HPP

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)"oatpp/codegen/DTO_define.hpp"

class StatusDTO : public oatpp::DTO {

    DTO_INIT(StatusDTO, DTO)

    DTO_FIELD_INFO(status) {
        info->description = "HTTP status";
    }
    DTO_FIELD(UInt16 , status);

    DTO_FIELD_INFO(message) {
        info->description = "Verbose message";
    }
    DTO_FIELD(String, message);

};

#include OATPP_CODEGEN_END(DTO)"oatpp/codegen/DTO_define.hpp"

#endif //CRUD_STATUSDTO_HPP
