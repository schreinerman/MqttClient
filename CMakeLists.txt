cmake_minimum_required(VERSION 3.7)
project(mqttclient)

set(CMAKE_CXX_STANDARD 11)
set(CAMAKE_C_STANDARD 11)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

set(SOURCE_FILES MqttClient/main.c)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

FIND_PACKAGE(Mosquitto)

if (NOT DEFINED STATIC_BUILD)
	find_package(Mosquitto REQUIRED)
	include_directories(${MOSQUITTO_INCLUDE_DIRS})
	message(STATUS "libmosquitto: ${MOSQUITTO_INCLUDE_DIRS} ${MOSQUITTO_LIBRARIES}")
        target_link_libraries (${PROJECT_NAME} ${MOSQUITTO_LIBRARIES})
endif()

#
# Use for CPP mosquittolib
#
#if (MOSQUITTO_FOUND)
#  include_directories(${MOSQUITTO_INCLUDE_DIR})
#  target_link_libraries (${PROJECT_NAME} ${MOSQUITTO_LIBRARIES})
#endif (MOSQUITTO_FOUND)


set(CMAKE_CXX_FLAGS "-g -")

# ==================== PATHS TO SPECIFY! ==================== #


