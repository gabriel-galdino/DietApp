cmake_minimum_required(VERSION 3.5)

add_library(presentation OBJECT
  pages/enter_page.cc
  pages/initial_page.cc
  pages/register_page.cc
  pages/create_page.cc
  nutritional_model.cc
  presentation.cc
)
target_compile_definitions(presentation PRIVATE
    __WXGTK__=1
    __WXGTK3__=1
    _FILE_OFFSET_BITS=64
    wxUSE_GUI=1
)
target_compile_options(presentation PRIVATE ${WX_CFLAGS_OTHER})
target_include_directories (presentation PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${WXWIDGETS_INCLUDE_DIRS})
target_link_libraries(presentation PRIVATE ${WXWIDGETS_LIBRARIES})
