#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_RESULT_SET_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_RESULT_SET_H

#include <string>

#include <wx/wxsqlite3.h>

#include "database_result_set.h"
#include "wx/wxsqlite3.h"

class SQLiteResultSet : public DatabaseResultSet {
 public:
  SQLiteResultSet(wxSQLite3ResultSet result_set);
  ~SQLiteResultSet() = default;
  bool NextRow() override;
  int GetInt(const std::string& column_name) override;
  std::string GetString(const std::string& column_name) override;
  double GetDouble(const std::string& column_name) override;

 private:
  wxSQLite3ResultSet result_set_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_RESULT_SET_H
