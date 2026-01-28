#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_DATABASE_ADAPTER_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_DATABASE_ADAPTER_H

#include <memory>
#include <string>

#include <wx/filename.h>
#include <wx/wxsqlite3.h>

#include "database_adapter.h"
#include "database_statement.h"

class SQLiteDatabaseAdapter : public DatabaseAdapter {
 public:
  SQLiteDatabaseAdapter() = default;

  ~SQLiteDatabaseAdapter() = default;

  bool Initialize(wxFileName& database) override;

  bool Shutdown() override;

  std::unique_ptr<DatabaseStatement> Prepare(const std::string& sql) override;

  // bool Execute(const std::string& query) override;

  // std::unique_ptr<QueryResult> Select(const std::string& query) override;

  // std::string GetLastError() override;

  long long GetLastInsertId() override;

  bool BeginTransaction() override;

  bool CommitTransaction() override;

  bool RollbackTransaction() override;

 private:
  wxSQLite3Database database_;
  std::string last_error_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_DATABASE_ADAPTER_H
