#include "database/sqlite_statement.h"

#include <string>

#include <wx/string.h>
#include <wx/wxsqlite3.h>

SQLiteStatement::SQLiteStatement(wxSQLite3Statement stmt)
    : stmt_(std::move(stmt)) {}

void SQLiteStatement::Bind(const std::string& param_name,
                           const std::string& value) {
  int index = stmt_.GetParamIndex(wxString(param_name));
  stmt_.Bind(index, wxString(value));
}

void SQLiteStatement::Bind(const std::string& param_name, int value) {
  int index = stmt_.GetParamIndex(wxString(param_name));
  stmt_.Bind(index, value);
}

bool SQLiteStatement::Execute() {
  return stmt_.ExecuteUpdate();
}