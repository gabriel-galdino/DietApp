cmake_minimum_required(VERSION 3.1)

project(DietApp VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

execute_process(
  COMMAND wx-config --version
  OUTPUT_VARIABLE WX_DETECTED_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(WX_DETECTED_VERSION VERSION_LESS "3.2.4")
  message(FATAL_ERROR "wxWidgets 3.2.4+ required. Version found: ${WX_DETECTED_VERSION}")
endif()

set(WXWIDGETS_PC_FILE "wxwidgets.pc")
set(LOCAL_PKG_DIR "$ENV{HOME}/.local/lib/pkgconfig")
set(SYSTEM_PKG_DIR "/usr/lib/x86_64-linux-gnu/pkgconfig")

if(NOT EXISTS "${LOCAL_PKG_DIR}")
    file(MAKE_DIRECTORY "${LOCAL_PKG_DIR}")
    message(STATUS "Creating pkgconfig directory in user home local: ${LOCAL_PKG_DIR}")
endif()

configure_file(
    "${CMAKE_SOURCE_DIR}/pkgconfig/libwx_gtk3u_core-3.2.pc"
    "${LOCAL_PKG_DIR}/libwx_gtk3u_core-3.2.pc"
    COPYONLY
)

find_package(PkgConfig REQUIRED)
if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config not found! Install it with:\n"
            "sudo apt install pkg-config")
endif()

set(ENV{PKG_CONFIG_PATH} "${LOCAL_PKG_DIR}:$ENV{PKG_CONFIG_PATH}")
pkg_check_modules(WXWIDGETS REQUIRED libwx_gtk3u_core-3.2)
if(NOT WXWIDGETS_FOUND)
    message(FATAL_ERROR "wxWidgets not found! To do checking:\n"
            "1. Installation: sudo apt install libwxgtk3.2-dev\n"
          "2. Search paths: $ENV{PKG_CONFIG_PATH}")
endif()

pkg_check_modules(WXSQLITE3 REQUIRED wxsqlite3-3.2)
if(NOT WXSQLITE3_FOUND)
    message(FATAL_ERROR "wxSQLite3 not found! To do checking:\n"
            "1. Installation: sudo apt install libwxsqlite3-3.2-dev\n"
          "2. Search paths: $ENV{PKG_CONFIG_PATH}")
endif()

add_subdirectory(src)