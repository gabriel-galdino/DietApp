cmake_minimum_required(VERSION 3.1)

add_executable(DietApp
  my_app.cc
  my_frame.cc
)
target_compile_definitions(DietApp PRIVATE
    __WXGTK__=1
    __WXGTK3__=1
    _FILE_OFFSET_BITS=64
    wxUSE_GUI=1
)
target_compile_options(DietApp PRIVATE ${WX_CFLAGS_OTHER})
target_include_directories (DietApp PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${WXWIDGETS_INCLUDE_DIRS})
target_link_libraries(DietApp PRIVATE ${WXWIDGETS_LIBRARIES})