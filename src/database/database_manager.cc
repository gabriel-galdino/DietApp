#include <memory>

#include "database/database_adapter.h"
#include "database/database_manager.h"
#include "database/sqlite_database_adapter.h"

std::shared_ptr<DatabaseAdapter> DatabaseManager::CreateAdapter() {
  return std::make_shared<SQLiteDatabaseAdapter>();
}