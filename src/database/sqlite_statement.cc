#include "database/sqlite_statement.h"

#include <string>

#include <wx/string.h>
#include <wx/wxsqlite3.h>

#include "database/sqlite_result_set.h"

SQLiteStatement::SQLiteStatement(wxSQLite3Statement stmt)
    : stmt_(std::move(stmt)) {}

void SQLiteStatement::Bind(const std::string& param_name,
                           const std::string& value) {
  int index = stmt_.GetParamIndex(wxString::FromUTF8(param_name));
  stmt_.Bind(index, wxString::FromUTF8(value));
}

void SQLiteStatement::Bind(const std::string& param_name, int value) {
  int index = stmt_.GetParamIndex(wxString::FromUTF8(param_name));
  stmt_.Bind(index, value);
}

void SQLiteStatement::Bind(const std::string& param_name, double value) {
  int index = stmt_.GetParamIndex(wxString::FromUTF8(param_name));
  stmt_.Bind(index, value);
}

bool SQLiteStatement::Execute() {
  return stmt_.ExecuteUpdate();
}

std::unique_ptr<DatabaseResultSet> SQLiteStatement::ExecuteQuery() {
  auto result_set = stmt_.ExecuteQuery();
  result_set_ = std::make_unique<SQLiteResultSet>(result_set);
  return std::move(result_set_);
}