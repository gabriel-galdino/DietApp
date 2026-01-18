cmake_minimum_required(VERSION 3.5)

add_executable(presentation_test
  presentation_test.cc
  wx/testableframe.cc
)
target_compile_definitions(presentation_test PRIVATE
    __WXGTK__=1
    __WXGTK3__=1
    _FILE_OFFSET_BITS=64
    wxUSE_GUI=1
    wxUSE_UIACTIONSIMULATOR=1
)
target_compile_options(presentation_test PRIVATE ${WX_CFLAGS_OTHER})
target_include_directories (presentation_test PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${WXWIDGETS_INCLUDE_DIRS}
  ${CATCH2_INCLUDE_DIRS})
target_link_libraries(presentation_test PRIVATE
  presentation
  ${WXWIDGETS_LIBRARIES}
  ${CATCH2_LIBRARIES})
