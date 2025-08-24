cmake_minimum_required(VERSION 3.1)

add_library(database
  database_manager.cc
  sqlite_database_adapter.cc
  user_repository.cc
)
target_compile_definitions(database PRIVATE
    __WXGTK__=1
    __WXGTK3__=1
    _FILE_OFFSET_BITS=64
    wxUSE_GUI=1
)
target_include_directories (database PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${WXSQLITE3_INCLUDE_DIRS}
  ${WXWIDGETS_INCLUDE_DIRS})
target_link_libraries(database PRIVATE ${WXSQLITE3_LIBRARIES})