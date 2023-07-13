# Find the native libmosquitto includes and libraries
#
#  Mosquitto_INCLUDE_DIR  - where to find mosquitto.h, etc.
#  Mosquitto_LIBRARIES    - List of libraries when using libmosquitto.
#  Mosquitto_FOUND        - True if libmosquitto found.
#  Mosquitto_FIND_QUIETLY - True if libmosquitto had been already found.

if(Mosquitto_FOUND)
    set(Mosquitto_FIND_QUIETLY TRUE)
endif()

find_path(Mosquitto_INCLUDE_DIR mosquitto.h)

find_library(Mosquitto_LIBRARY NAMES libmosquitto mosquitto)
find_library(Mosquitto_PP_LIBRARY NAMES libmosquittopp mosquittopp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    Mosquitto
    DEFAULT_MSG
    Mosquitto_LIBRARY Mosquitto_PP_LIBRARY Mosquitto_INCLUDE_DIR)

if(Mosquitto_FOUND)
    set(Mosquitto_LIBRARIES ${Mosquitto_LIBRARY} ${Mosquitto_PP_LIBRARY})
else()
    set(Mosquitto_LIBRARIES)
endif()

mark_as_advanced(
    Mosquitto_INCLUDE_DIR
    Mosquitto_LIBRARY
    Mosquitto_PP_LIBRARY)

message(STATUS Mosquitto_LIBRARY=${Mosquitto_LIBRARY})
message(STATUS Mosquitto_PP_LIBRARY=${Mosquitto_PP_LIBRARY})