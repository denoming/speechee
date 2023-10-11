find_package(PkgConfig)

pkg_check_modules(Mosquitto REQUIRED IMPORTED_TARGET libmosquitto)
