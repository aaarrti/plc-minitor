//
// Created by Artem Sereda on 20.06.21.
//

#include "PlcClient.hpp"
#include "future"
#include "AppComponent.hpp"

using namespace std;
using namespace libconfig;

extern char *metricNames[];


PlcClient::PlcClient() {
    OATPP_COMPONENT(std::shared_ptr<libconfig::Config>, _config);
    this->config = _config;
}

void PlcClient::connect_to_plc() {
    char *ip = std::getenv("GATEWAY_IP");
    if (ip == nullptr) {
        throw runtime_error("#### GATEWAY_IP undefined ####");
    }
    spdlog::debug("#### Testing PLC connection to [{}]", ip);
    auto handler = new CEasyPLCHandler(RTS_INVALID_HANDLE);
    int gatewayPort = config->lookup("gatewayPort");
    int plcPort = config->lookup("plcPort");
    std::future<long> future = std::async(std::launch::async, [handler, ip, gatewayPort, plcPort]() {
        return handler->ConnectTcpipViaGateway3(ip, gatewayPort, ip, plcPort, 1, PLCHANDLER_USE_DEFAULT);
    });
    int timeOutMs = config->lookup("plcConnectTimeOutMs");
    std::future_status status = future.wait_for(std::chrono::seconds(timeOutMs));
    if (status == std::future_status::timeout) {
        throw runtime_error("Connect to PLC timed out");
    }
    auto result = future.get();
    if (result == RESULT_OK) {
        spdlog::debug("#### Connected successfully ####");
        this->plcHandler = handler;
    } else {
        spdlog::error("#### Failed to connect, {} ####", decode_error_code(result));
        throw runtime_error("#### Failed to connect ####");
    }
}

HCYCLIST PlcClient::cyc_define_vars(CPLCHandlerCallback *callback) {
    auto ulFlags = 0;
    int scanPeriodMs = config->lookup("scanPeriodMs");
    HCYCLIST hCycVarList = plcHandler->CycDefineVarList(metricNames, 2, scanPeriodMs, nullptr, callback, 0, ulFlags);
    return hCycVarList;
}


void PlcClient::sync_write_windspeed(float newValue) {
    auto *ppbyValues = (unsigned char *) &newValue;
    char *name = "Application.PLC_PRG.rWindSpeed";
    auto result = plcHandler->SyncWriteVarsToPlc(&name, 1, &ppbyValues, nullptr);
    if (result != RESULT_OK) {
        spdlog::error("Error while writing windspeed, {}", decode_error_code(result));
    }
}


std::string PlcClient::decode_error_code(long code) {
    switch (code) {
        case RESULT_FAILED:
            return "RESULT_FAILED";
        case RESULT_OK:
            return "RESULT_OK";
        case RESULT_PLC_NOT_CONNECTED:
            return "RESULT_PLC_NOT_CONNECTED";
        case RESULT_PLC_LOGIN_FAILED:
            return "RESULT_PLC_LOGIN_FAILED";
        case RESULT_PLC_NO_CYCLIC_LIST_DEFINED:
            return "RESULT_PLC_NO_CYCLIC_LIST_DEFINED";
        case RESULT_PLCHANDLER_INACTIVE:
            return "RESULT_PLCHANDLER_INACTIVE";
        case RESULT_LOADING_SYMBOLS_FAILED:
            return "RESULT_LOADING_SYMBOLS_FAILED		";
        case RESULT_ITF_NOT_SUPPORTED            :
            return "RESULT_ITF_NOT_SUPPORTED			";
        case RESULT_COMM_FATAL                    :
            return "RESULT_COMM_FATAL					";
        case RESULT_NO_CONFIGURATION            :
            return "RESULT_NO_CONFIGURATION			";
        case RESULT_INVALID_PARAMETER            :
            return "RESULT_INVALID_PARAMETER			";
        case RESULT_ITF_FAILED                    :
            return "RESULT_ITF_FAILED					";
        case RESULT_NOT_SUPPORTED                :
            return "RESULT_NOT_SUPPORTED				";
        case RESULT_EXCEPTION                    :
            return "RESULT_EXCEPTION					";
        case RESULT_TIMEOUT                           :
            return "RESULT_TIMEOUT						  ";
        case RESULT_STILL_CONNECTED                   :
            return "RESULT_STILL_CONNECTED				  ";
        case RESULT_RECONNECTTHREAD_STILL_ACTIVE:
            return "RESULT_RECONNECTTHREAD_STILL_ACTIVE";
        case RESULT_PLC_NOT_CONNECTED_SYMBOLS_LOADED :
            return "RESULT_PLC_NOT_CONNECTED_SYMBOLS_LOADED";
        case RESULT_NO_UPDATE                    :
            return "RESULT_NO_UPDATE					";
        case RESULT_OCX_CONVERSION_FAILED        :
            return "RESULT_OCX_CONVERSION_FAILED		";
        case RESULT_TARGETID_MISMATCH            :
            return "RESULT_TARGETID_MISMATCH			";
        case RESULT_NO_OBJECT                    :
            return "RESULT_NO_OBJECT					";
        case RESULT_COMPONENTS_NOT_LOADED        :
            return "RESULT_COMPONENTS_NOT_LOADED		";
        case RESULT_BUSY                        :
            return "RESULT_BUSY						";
        case RESULT_DISABLED                    :
            return "RESULT_DISABLED					";
        case RESULT_PLC_FAILED                    :
            return "RESULT_PLC_FAILED					";
        case RESULT_INVALID_SYMBOL                :
            return "RESULT_INVALID_SYMBOL				";
        case RESULT_BUFFER_TOO_SMALL            :
            return "RESULT_BUFFER_TOO_SMALL			";
        case RESULT_NO_PROJECT                    :
            return "RESULT_NO_PROJECT					";
        case RESULT_FILE_ERROR                    :
            return "RESULT_FILE_ERROR					";
        case RESULT_RETAIN_MISMATCH                   :
            return "RESULT_RETAIN_MISMATCH				  ";
        case RESULT_NO_ACCESS_RIGHTS            :
            return "RESULT_NO_ACCESS_RIGHTS			";
        case RESULT_DUPLICATE_PLC_NAME            :
            return "RESULT_DUPLICATE_PLC_NAME			";
        case RESULT_SIZE_MISMATCH                :
            return "RESULT_SIZE_MISMATCH				";
        case RESULT_LIST_NO_WRITE_ACCESS        :
            return "RESULT_LIST_NO_WRITE_ACCESS		";
        case RESULT_CONSISTENT_ACCESS_TIMEOUT    :
            return "RESULT_CONSISTENT_ACCESS_TIMEOUT	";
        case RESULT_SYNC_CONSISTENT_ACCESS_DENIED:
            return "RESULT_SYNC_CONSISTENT_ACCESS_DENIED";
        case RESULT_INVALID_ASCII_STRING        :
            return "RESULT_INVALID_ASCII_STRING		";
        case RESULT_INVALID_STRING_LENGTH        :
            return "RESULT_INVALID_STRING_LENGTH		";
        case RESULT_OUTOFMEMORY                       :
            return "RESULT_OUTOFMEMORY					  ";
        case RESULT_NO_FILE                           :
            return "RESULT_NO_FILE						  ";
        case RESULT_APPLICATION_NOT_IN_STOP           :
            return "RESULT_APPLICATION_NOT_IN_STOP		  ";
        case RESULT_APPLICATION_NOT_IN_RUN        :
            return "RESULT_APPLICATION_NOT_IN_RUN		";
        case RESULT_OPERATIONMODE_NOT_IN_DEBUG    :
            return "RESULT_OPERATIONMODE_NOT_IN_DEBUG	";
        case RESULT_BACKUP_RESTORE_NOT_SUPPORTED:
            return "RESULT_BACKUP_RESTORE_NOT_SUPPORTED";
        case RESULT_PLC_INCONSISTENT_STATE        :
            return "RESULT_PLC_INCONSISTENT_STATE		";
        case RESULT_PLC_INCOMPATIBLE            :
            return "RESULT_PLC_INCOMPATIBLE			";
        case RESULT_PLC_VERSION_INCOMPATIBLE    :
            return "RESULT_PLC_VERSION_INCOMPATIBLE	";
        case RESULT_RETAIN_ERROR                :
            return "RESULT_RETAIN_ERROR				";
        case RESULT_APPLICATIONS_LOAD_ERROR           :
            return "RESULT_APPLICATIONS_LOAD_ERROR		  ";
        case RESULT_APPLICATIONS_START_ERROR    :
            return "RESULT_APPLICATIONS_START_ERROR	";
        case RESULT_FILETRANSFER_ERROR            :
            return "RESULT_FILETRANSFER_ERROR			";
        case RESULT_OPERATION_DENIED            :
            return "RESULT_OPERATION_DENIED			";
        case RESULT_FORCES_ACTIVE                :
            return "RESULT_FORCES_ACTIVE				";
        case RESULT_META_VERSION_MISMATCH        :
            return "RESULT_META_VERSION_MISMATCH		";
        case RESULT_PROTOCOL_MISMATCH            :
            return "RESULT_PROTOCOL_MISMATCH			";
        case RESULT_USER_ACCESS_DENIED            :
            return "RESULT_USER_ACCESS_DENIED			";
        case RESULT_CALL_AGAIN                    :
            return "RESULT_CALL_AGAIN					";
        case RESULT_NO_USR_MGT                    :
            return "RESULT_NO_USR_MGT					";
        case RESULT_ACTIVATE_USERMGMT            :
            return "RESULT_ACTIVATE_USERMGMT";
    }
    return std::string();
}
