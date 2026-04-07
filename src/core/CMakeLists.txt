cmake_minimum_required(VERSION 3.5)

add_library(application OBJECT
  application.cc
  domain/user.cc
  domain/meal.cc
  xlsx_service.cc
  wx_bootstrap_app.cc
)
target_compile_definitions(application PRIVATE
    __WXGTK__=1
    __WXGTK3__=1
    _FILE_OFFSET_BITS=64
    wxUSE_GUI=1
)
target_include_directories (application PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/OpenXLSX
  ${CMAKE_BINARY_DIR}/OpenXLSX
  ${WXWIDGETS_INCLUDE_DIRS})
target_link_libraries (application PRIVATE
  database
  OpenXLSX::OpenXLSX
  ${WXWIDGETS_LIBRARIES})