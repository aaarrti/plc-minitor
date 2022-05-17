cmake_minimum_required(VERSION 3.16)
project(plc_monitor_v2)
set(CMAKE_CXX_STANDARD 20)


find_package(Threads REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.68.0
        COMPONENTS REQUIRED
        stacktrace_basic
        stacktrace_backtrace
        stacktrace_addr2line
        stacktrace_noop
)

set(PLC_HANDLER_LIB "${PROJECT_SOURCE_DIR}/lib/libCmpPLCHandler-x64.a")
set(prometheus-cpp_DIR "${PROJECT_SOURCE_DIR}/lib/prometheus-cpp/_build/deploy/usr/local/lib/cmake/prometheus-cpp")
set(CONFIG_LIB "${PROJECT_SOURCE_DIR}/lib/libconfig-1.7.3/lib/.libs/libconfig++.a")


find_package(spdlog         REQUIRED)
find_package(oatpp          REQUIRED)
find_package(oatpp-swagger  REQUIRED)
find_package(prometheus-cpp REQUIRED)

set(HEADER_SEARCH_PATHS
        "${PROJECT_SOURCE_DIR}/include/plchandler"
        "${PROJECT_SOURCE_DIR}/include/"
        "${PROJECT_SOURCE_DIR}/lib/libconfig-1.7.3/lib/"
        "${PROJECT_SOURCE_DIR}/include/swagger/"
        "/usr/include/python3.8/"
        )


file(GLOB SOURCES CONFIGURE_DEPENDS "src/main.cpp" "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
include_directories(${HEADER_SEARCH_PATHS})

target_link_libraries(
        ${PROJECT_NAME}
        ${PLC_HANDLER_LIB}
        ${CMAKE_DL_LIBS}
        ${CONFIG_LIB}
        spdlog::spdlog
        oatpp::oatpp oatpp::oatpp-swagger
        prometheus-cpp::push prometheus-cpp::pull
        ${Boost_STACKTRACE_BASIC_LIBRARY}
        ${Boost_STACKTRACE_BACKTRACE_LIBRARY}
        ${Boost_STACKTRACE_ADDR2LINE_LIBRARY}
        ${Boost_STACKTRACE_NOOP_LIBRARY}
        pthread dl rt
        python3.8
)


add_definitions(
        -DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res"
)
