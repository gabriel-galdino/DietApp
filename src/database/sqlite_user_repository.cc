#include "database/sqlite_user_repository.h"

#include <memory>
#include <string>

#include "core/domain/user.h"
#include "database/database_adapter.h"

SQLiteUserRepository::SQLiteUserRepository(
    std::shared_ptr<DatabaseAdapter> db_adapter)
    : db_adapter_(db_adapter) {}

User& SQLiteUserRepository::GetUser(const std::string& name) {
  std::string dummy_string;
  User dummy(dummy_string);
  return dummy;
}

bool SQLiteUserRepository::AddUser(const User& user) {
  auto stmt = db_adapter_->Prepare(kInsertUserSql);
  stmt->Bind(":name", user.name());
  return stmt->Execute();
}

bool SQLiteUserRepository::DelUser(const User& user) {
  return true;
}

bool SQLiteUserRepository::UpdateUser(const User& user) {
  return true;
}

bool SQLiteUserRepository::ValidateCredentials() {
  return false;
}