#ifndef DIETAPP_INCLUDE_DATABASE_DATABASE_ADAPTER_H
#define DIETAPP_INCLUDE_DATABASE_DATABASE_ADAPTER_H

#include <memory>
#include <string>

#include <wx/filename.h>

#include "database_statement.h"

class DatabaseAdapter {
 public:
  virtual ~DatabaseAdapter() = default;

  virtual bool Initialize(wxFileName& database) = 0;

  virtual bool Shutdown() = 0;

  virtual std::unique_ptr<DatabaseStatement> Prepare(
      const std::string& sql) = 0;

  // virtual bool Execute(const std::string& query) = 0;
  //
  // virtual std::unique_ptr<QueryResult> Select(const std::string& query) = 0;
  //
  // virtual std::string GetLastError() = 0;
  //
  virtual long long GetLastInsertId() = 0;

  virtual bool BeginTransaction() = 0;

  virtual bool CommitTransaction() = 0;

  virtual bool RollbackTransaction() = 0;
};

#endif  // DIETAPP_INCLUDE_DATABASE_DATABASE_ADAPTER_H