if(Boost_DEBUG)
    message(STATUS "Boost debug mode is active")
    find_package(Boost 1.78.0 REQUIRED COMPONENTS ALL)
else()
    find_package(Boost 1.78.0 REQUIRED COMPONENTS program_options system)
endif()
