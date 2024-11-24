set(Boost_NO_WARN_NEW_VERSIONS ON)
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.81.0
    REQUIRED
    COMPONENTS json
    CONFIG)
