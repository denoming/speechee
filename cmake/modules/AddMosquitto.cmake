find_package(Mosquitto REQUIRED)

if(Mosquitto_FOUND)
    set(_TARGET_NAME Mosquitto)
    add_library(${_TARGET_NAME} SHARED IMPORTED GLOBAL)
    add_library(${_TARGET_NAME}::${_TARGET_NAME} ALIAS ${_TARGET_NAME})
    set_target_properties(${_TARGET_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${Mosquitto_INCLUDE_DIR})
    set_target_properties(${_TARGET_NAME} PROPERTIES IMPORTED_LOCATION ${Mosquitto_LIBRARY})
endif()