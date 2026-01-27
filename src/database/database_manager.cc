#include <memory>

#include "database/database_adapter.h"
#include "database/database_manager.h"

std::unique_ptr<DatabaseAdapter> DatabaseManager::CreateAdapter() {
  return nullptr;
}