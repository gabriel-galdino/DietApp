#include "database/sqlite_result_set.h"

#include <string>

#include <wx/wxsqlite3.h>

SQLiteResultSet::SQLiteResultSet(wxSQLite3ResultSet result_set)
    : result_set_(result_set) {}
bool SQLiteResultSet::NextRow() {
  return result_set_.NextRow();
}
int SQLiteResultSet::GetInt(const std::string& column_name) {
  return result_set_.GetInt(wxString::FromUTF8(column_name));
}
std::string SQLiteResultSet::GetString(const std::string& column_name) {
  return result_set_.GetAsString(wxString::FromUTF8(column_name)).utf8_string();
}
double SQLiteResultSet::GetDouble(const std::string& column_name) {
  return result_set_.GetDouble(wxString::FromUTF8(column_name), 0);
}
