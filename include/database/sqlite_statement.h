#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_STATEMENT_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_STATEMENT_H

#include <memory>
#include <string>

#include <wx/wxsqlite3.h>

#include "database_result_set.h"
#include "database_statement.h"

class SQLiteStatement : public DatabaseStatement {
 public:
  explicit SQLiteStatement(wxSQLite3Statement stmt);

  void Bind(const std::string& param_name, const std::string& value) override;

  void Bind(const std::string& param_name, int value) override;

  bool Execute() override;

  std::unique_ptr<DatabaseResultSet> ExecuteQuery() override;

 private:
  wxSQLite3Statement stmt_;
  std::unique_ptr<DatabaseResultSet> result_set_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_STATEMENT_H
