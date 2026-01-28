#ifndef DIETAPP_INCLUDE_DATABASE_DATABASE_MANAGER_H
#define DIETAPP_INCLUDE_DATABASE_DATABASE_MANAGER_H

#include <memory>

#include "database_adapter.h"

class DatabaseManager {
 public:
  static std::shared_ptr<DatabaseAdapter> CreateAdapter();
};

#endif  // DIETAPP_INCLUDE_DATABASE_DATABASE_MANAGER_H
