find_package(PkgConfig)

pkg_check_modules(Systemd REQUIRED IMPORTED_TARGET libsystemd)

find_package(sdbus-c++ REQUIRED)
