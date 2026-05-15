#ifndef DIETAPP_INCLUDE_DATABASE_DATABASE_ADAPTER_H
#define DIETAPP_INCLUDE_DATABASE_DATABASE_ADAPTER_H

#include <memory>
#include <string>

#include <wx/filename.h>

#include "database_statement.h"

class DatabaseAdapter {
 public:
  virtual ~DatabaseAdapter() = default;

  virtual void Initialize(wxFileName& database) = 0;

  virtual void Shutdown() = 0;

  virtual std::unique_ptr<DatabaseStatement> Prepare(
      const std::string& sql) = 0;

  virtual long long GetLastInsertId() = 0;

  virtual void BeginTransaction() = 0;

  virtual void CommitTransaction() = 0;

  virtual void RollbackTransaction() = 0;
};

#endif  // DIETAPP_INCLUDE_DATABASE_DATABASE_ADAPTER_H
