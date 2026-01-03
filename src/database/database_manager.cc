#include <memory>
#include <string>

#include "database/database_adapter.h"
#include "database/database_manager.h"
#include "database/sqlite_database_adapter.h"

std::unique_ptr<DatabaseAdapter> DatabaseManager::CreateAdapter() {
  return std::unique_ptr<SQLiteDatabaseAdapter>(new SQLiteDatabaseAdapter());
}