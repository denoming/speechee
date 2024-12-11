set(CPACK_DEB_COMPONENT_INSTALL ON)

set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
if(DEFINED VCPKG_INSTALLED_DIR)
    list(APPEND CPACK_DEBIAN_PACKAGE_SHLIBDEPS_PRIVATE_DIRS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")
endif()

#
# Runtime package configuration
#

set(CPACK_DEBIAN_SPEECHEE_RUNTIME_PACKAGE_NAME "speechee")
set(CPACK_DEBIAN_SPEECHEE_RUNTIME_FILE_NAME DEB-DEFAULT)
if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CPACK_DEBIAN_SPEECHEE_RUNTIME_DEBUGINFO_PACKAGE ON)
endif()

string(CONCAT CPACK_DEBIAN_SPEECHEE_RUNTIME_PACKAGE_DEPENDS
    "libjarvisto (>= 0.3.5), "
    "libvoxer (>= 0.2.3), "
    "libboost-json1.81.0 (>= 1.81.0), "
    "libglibmm-2.68-1 (>= 2.74.0), "
    "libconfig++9v5 (>= 1.5), "
    "libsigc++-3.0-0 (>= 3.4.0), "
    "gstreamer1.0-plugins-base (>= 1.22.0), "
    "gstreamer1.0-plugins-good (>= 1.22.0), "
    "gstreamer1.0-alsa (>= 1.22.0), "
    "gstreamer1.0-pulseaudio (>= 1.22.0)"
)

configure_file("cmake/generators/deb/postinst.in"
               "cmake/generators/deb/postinst" @ONLY)
configure_file("cmake/generators/deb/prerm.in"
               "cmake/generators/deb/prerm" @ONLY)
configure_file("cmake/generators/deb/postrm.in"
               "cmake/generators/deb/postrm" @ONLY)
set(CPACK_DEBIAN_SPEECHEE_RUNTIME_PACKAGE_CONTROL_EXTRA
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/generators/deb/postinst"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/generators/deb/prerm"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/generators/deb/postrm"
)
