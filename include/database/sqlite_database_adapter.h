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

  void Initialize(wxFileName& database) override;

  void Shutdown() override;

  std::unique_ptr<DatabaseStatement> Prepare(const std::string& sql) override;

  long long GetLastInsertId() override;

  void BeginTransaction() override;

  void CommitTransaction() override;

  void RollbackTransaction() override;

 private:
  wxSQLite3Database database_;
  std::string last_error_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_DATABASE_ADAPTER_H
