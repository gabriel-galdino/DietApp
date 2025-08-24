#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_DATABASE_ADAPTER_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_DATABASE_ADAPTER_H

#include <wx/wxsqlite3.h>

#include "database_adapter.h"

class SQLiteDatabaseAdapter : public DatabaseAdapter {
 public:
  SQLiteDatabaseAdapter() = default;
  ~SQLiteDatabaseAdapter() override = default;

  bool Connect(const std::string& databasePath) override;
  bool Execute(const std::string& query) override;
  std::unique_ptr<QueryResult> Select(const std::string& query) override;
  int GetLastInsertId() override;
  std::string GetLastError() override;

  bool BeginTransaction() override;
  bool CommitTransaction() override;
  bool RollbackTransaction() override;

 private:
  wxSQLite3Database database_;
  std::string last_error_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_DATABASE_ADAPTER_H
