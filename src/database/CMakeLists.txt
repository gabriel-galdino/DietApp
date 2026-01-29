cmake_minimum_required(VERSION 3.5)

add_library(database OBJECT
  database_manager.cc
  sqlite_database_adapter.cc
  sqlite_user_repository.cc
  sqlite_meal_repository.cc
  sqlite_result_set.cc
  sqlite_statement.cc
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