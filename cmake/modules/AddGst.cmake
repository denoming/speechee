find_package(PkgConfig)

pkg_check_modules(Gst REQUIRED IMPORTED_TARGET gstreamer-1.0)
pkg_check_modules(GstApp REQUIRED IMPORTED_TARGET gstreamer-app-1.0)
