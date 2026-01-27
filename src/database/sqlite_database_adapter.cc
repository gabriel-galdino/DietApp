#include <memory>
#include <string>

#include "database/database_adapter.h"
#include "database/sqlite_database_adapter.h"

bool SQLiteDatabaseAdapter::Connect(const std::string& databasePath) {
  return false;
}

bool SQLiteDatabaseAdapter::Execute(const std::string& query) {
  return false;
}

std::unique_ptr<QueryResult> SQLiteDatabaseAdapter::Select(
    const std::string& query) {
  return nullptr;
}
int SQLiteDatabaseAdapter::GetLastInsertId() {
  return -1;
}

std::string SQLiteDatabaseAdapter::GetLastError() {
  return "";
}

bool SQLiteDatabaseAdapter::BeginTransaction() {
  return false;
}

bool SQLiteDatabaseAdapter::CommitTransaction() {
  return false;
}

bool SQLiteDatabaseAdapter::RollbackTransaction() {
  return false;
}