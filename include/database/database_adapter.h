#ifndef DIETAPP_INCLUDE_DATABASE_DATABASE_ADAPTER_H
#define DIETAPP_INCLUDE_DATABASE_DATABASE_ADAPTER_H

#include <memory>
#include <string>
#include <vector>

struct QueryResult;

class DatabaseAdapter {
 public:
  virtual ~DatabaseAdapter() = default;
  virtual bool Connect(const std::string& databasePath) = 0;

  virtual bool Execute(const std::string& query) = 0;
  virtual std::unique_ptr<QueryResult> Select(const std::string& query) = 0;
  virtual int GetLastInsertId() = 0;
  virtual std::string GetLastError() = 0;

  // Transcation Methods
  virtual bool BeginTransaction() = 0;
  virtual bool CommitTransaction() = 0;
  virtual bool RollbackTransaction() = 0;
};

// Query Result Structure
struct QueryResult {
  std::vector<std::vector<std::string>> rows;
  std::vector<std::string> columnNames;
};

#endif  // DIETAPP_INCLUDE_DATABASE_DATABASE_ADAPTER_H